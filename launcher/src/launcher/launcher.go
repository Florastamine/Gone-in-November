/* launcher.go */
/*
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                     Version 2, December 2004
 *
 *  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 *  Everyone is permitted to copy and distribute verbatim or modified
 *  copies of this license document, and changing it is allowed as long
 *  as the name is changed.
 *
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *   0. You just DO WHAT THE FUCK YOU WANT TO.
 *
 * A simple Go-based game launcher that does the updating stuff before executing the game, including
 * downloading, manipulating and extracting (zipped) update files.
 *
 * _______________________________________________
 * + v0.1.0
 * - First working release.
 * _______________________________________________
 */
package main

import (
    "fmt"                   // .Println(), .Sprintf()
    "time"                  // .Now(), not necessary.
    "io/ioutil"             // .ReadFile()
    "strconv"               // .ParseInt(), .Atoi()
    "os"                    // .TempDir(), .Args[], .MkdirAll(), ...
    "os/exec"               // .Command(), .Output()
    "runtime"               // .GOOS, not necessary.
    "path/filepath"         // Extra fs utilities.

    "launcher/common"
    "launcher/libhttp"
    "launcher/libzip"
)

func main() {
    common.SetDebug(true)
    libhttp.SetDebug(true)
    libzip.SetDebug(true)

    // Select our directory where the locker file will be created on.
    locker_path, _ := filepath.Abs(common.LockerPath)
    common.SetLockerDirectory(locker_path)
    common.SetLockerFileName(common.GameName + ".lock")

    // Select the directory where the temporary files will be downloaded/created on.
    common.SetDownloadDirectory(os.TempDir() + "\\")

    // Create the locker.
    err := common.CreateLocker()
    if !err {
        os.Exit(1)
    }

    // Print out some basic information.
    fmt.Println(common.GameName + " launcher" + " (" + os.Args[0] + "), running under " + runtime.GOOS)
    fmt.Println("Time is now ", time.Now())

    // Try to connect to the file server, download the remote versioning file, perform versioning comparision and get the updates.
    fmt.Println("Attempting to establish connection to the server...")
    r := libhttp.Connected()
    if r {
        fmt.Println("Successfully connected to the server. Checking for updates...")

        err_download, _ := libhttp.DownloadFile(common.FilePath, common.FileName, common.GetDownloadDirectory())
        if err_download != true {
            fmt.Println("Failed to download the definition file. Abort updating the game.")

            // Delete the file that was created by os.Create() during libhttp.DownloadFile().
            file := common.GetDownloadDirectory() + common.FileName
            if common.IsExist(file) {
                os.Remove(file)
            }
        } else { // Successfully downloaded the definition file.
            if(common.GetDebug()) {
                fmt.Println("[DEBUG LOG] Definition file downloaded at ", common.GetDownloadDirectory() + common.FileName)
            }

            // Extract the version number from the downloaded file, and compare it with our game version.

            // Retrieve the local version number. We get the local version first, if it fails, then do not
            // extract the remote version number.
            if common.IsExist(common.FileName) {
                var remote_version, local_version int = 0, 0

                data_remote, _ := ioutil.ReadFile(common.GetDownloadDirectory() + common.FileName)
                remote_version, _ = strconv.Atoi(string(data_remote))

                data_local, _ := ioutil.ReadFile(common.FileName)
                local_version, _ = strconv.Atoi(string(data_local))
                fmt.Println("Remote version : [", remote_version, "]; Local version: [", local_version, "].");

                // If the local version is lower than the version that was received from the remote server,
                // we perform the updating process.
                if remote_version > local_version  {
                    var update_pack_loc string = common.GameName + string(data_remote) + ".zip"           // Name of the ZIP package.
                    var update_pack_sav string = common.GetDownloadDirectory() + common.GameName + "\\"   // Temporary path to which the package will be downloaded.
                    var update_pack_len int64                                                             // Size of the package, in bytes.
                    var update_pack_err bool

                    fmt.Print("[Phase 1] Downloading update packages... [", update_pack_loc, "]")

                    update_pack_err, update_pack_len = libhttp.DownloadFile(common.FilePath, update_pack_loc, update_pack_sav)
                    if update_pack_err != true {
                        fmt.Println("Cannot download the update package. ")
                    } else {
                        // Print out the size of the update package.
                        fmt.Println("[", float64(update_pack_len / 1024), " KBs]")

                        fmt.Print("[Phase 2] Copying update files...")
                        _ = common.CopyFile("./" + update_pack_loc, update_pack_sav + update_pack_loc)
                        fmt.Println("done.")

                        // Because we are doing stuff in the temporary directory, and the main partition running out of space-kind-of-exception is very rare, we
                        // won't be catching errors here.
                        /*
                        _ = os.MkdirAll(update_pack_sav, os.ModeDir | os.ModeTemporary)
                        _ = os.MkdirAll(update_pack_sav + "package\\", os.ModeDir | os.ModeTemporary)
                        */

                        fmt.Print("[Phase 3] Extracting the package...")
                        if result := libzip.Extract("./" + update_pack_loc, "./") ; !result {
                            fmt.Println("Cannot extract the package")
                        }
                        fmt.Println("done.")

                        fmt.Print("[Phase 4] Cleaning up...")
                        if common.IsExist("./" + update_pack_loc) {
                            if common.GetDebug() {
                                fmt.Println("Deleting the update package... (", "./" + update_pack_loc, ")")
                            }

                            os.Remove("./" + update_pack_loc)
                        }
                        fmt.Println("done.")

                    }

                } else {
                    fmt.Println("Skipping the update process...")
                }
            }
        }

    } else {
        fmt.Println("You are not connected to the Internet. Abort updating the game.");
    }

    // Launch the game.
    fmt.Println("Running the game... [" + common.GetExecutableName() + "]")
    _, _ = exec.Command(common.GetExecutableName()).Output()

    // Release the locker.
    common.DestroyLocker()
}
