/* libhttp.go */
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
package libhttp

import (
    "fmt"
    "net/http"
    "os"
    "io"
)

var (
    Debug          bool   = false
)

func SetDebug(Flag bool) {
    if Flag {
        Debug = true
    } else {
        Debug = false
    }
}

func GetDebug() bool {
    return Debug
}

func Connected() bool {
    _, err := http.Get("https://www.dropbox.com/")
    if err != nil {
        if GetDebug() {
            fmt.Println("[DEBUG LOG] Probably your computer is not connected to the Internet, or the server has shut down, or there was a problem establishing a HTTP request.")
        }

        return false
    }

    return true
}

func DownloadFile(url string, file string, path string) (bool, int64) {
    data, err := os.Create(path + file)
    if err != nil {
        if GetDebug() {
            fmt.Println("[DEBUG LOG] There was a problem trying to create the file descriptor. Operation aborted.")
        }

        return false, 0
    }

    rep, err_http := http.Get(url + file)
    if err_http != nil {
        if GetDebug() {
            fmt.Println("[DEBUG LOG] There was a problem trying to download the file. Operation aborted.")
        }

        return false, 0
    }

    _, err_write := io.Copy(data, rep.Body)
    if err_write != nil {
        if GetDebug() {
            fmt.Println("[DEBUG LOG] There was a problem trying to finalizing the download. Operation aborted.")
        }

        return false, 0
    }

    return true, rep.ContentLength
}
