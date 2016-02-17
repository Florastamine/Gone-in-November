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
    "os"
    "runtime"
    "launcher/common"
    "launcher/libhttp"
    // "launcher/libzip"
)

func main() {
    fmt.Println(common.GameName + " launcher" + " (" + os.Args[0] + "), running under " + runtime.GOOS)
    fmt.Println("Time is now ", time.Now())
    fmt.Println("[" + common.GetExecutableName() + "]")
    fmt.Println("")

    fmt.Println("Attempting to establish connection to the server...")
    r := libhttp.Connected()
    if r {
        fmt.Println("Successfully connected to the server. Checking for updates...")
        common.SetDownloadDirectory(os.TempDir() + "\\")

        err_download := libhttp.DownloadFile(common.FilePath, common.FileName, common.GetDownloadDirectory())
        if err_download != true {
            fmt.Println("#2")
        }
    }

    fmt.Println(common.GetDownloadDirectory())
}
