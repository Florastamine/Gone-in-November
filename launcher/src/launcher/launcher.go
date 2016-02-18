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
 */
package main

import (
    "fmt"
    "time"
    "io/ioutil"
    "strconv"
    "os"
    "os/exec"
    "runtime"
    "path/filepath"

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

    // Create the locker.
    common.CreateLocker()

    // Print out some basic information.
    fmt.Println(common.GameName + " launcher" + " (" + os.Args[0] + "), running under " + runtime.GOOS)
    fmt.Println("Time is now ", time.Now())

    // Try to connect to the file server, download the remote versioning file, perform versioning comparision and get the updates.
    fmt.Println("Attempting to establish connection to the server...")
    r := libhttp.Connected()
    if r {
        fmt.Println("Successfully connected to the server. Checking for updates...")
        common.SetDownloadDirectory(os.TempDir() + "\\")

        err_download := libhttp.DownloadFile(common.FilePath, common.FileName, common.GetDownloadDirectory())
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

                if remote_version > local_version  {
                    fmt.Println("Downloading update packages...")
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
