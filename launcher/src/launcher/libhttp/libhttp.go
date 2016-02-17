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

func Connected() bool {
    _, err := http.Get("https://www.dropbox.com/")
    if err != nil {
        return false
    }

    return true
}

func DownloadFile(url string, file string, path string) bool {
    data, err := os.Create(path + file)
    if err != nil {
        fmt.Println("_1");
        return false
    }

    rep, err_http := http.Get(url + file)
    if err_http != nil {
        fmt.Println("_2");
        return false
    }

    _, err_write := io.Copy(data, rep.Body)
    if err_write != nil {
        fmt.Println("_3");
        return false
    }

    return true
}
