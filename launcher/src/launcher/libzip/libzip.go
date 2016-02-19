/* libzip.go */
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
package libzip

import (
    "io"
    "os"
    "fmt"
    "archive/zip"
    "path/filepath"
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

func Extract(src, loc string) bool {
    descriptor, err := zip.OpenReader(src)
    if err != nil {
        if GetDebug() {
            fmt.Println("[DEBUG LOG] There was a problem trying to open the update package. Operation aborted.")
        }

        return false
    }

    if GetDebug() {
        fmt.Println("[DEBUG LOG] libzip.Extract(): [", src, "; ", loc, "]")
    }

    defer func() {
        err := descriptor.Close()
        if err != nil {
            if GetDebug() {
                fmt.Println("[DEBUG LOG] There was an error trying to close the ZIP descriptor. Operation aborted.")
            }

            return
        }
    } ()

    _ = os.MkdirAll(loc, 0755)

    // Closure to address file descriptors issue with all the deferred .Close() methods
    __ExtractAndWrite__ := func(f *zip.File) bool {
        rc, err := f.Open()
        if err != nil {
            if GetDebug() {
                fmt.Println("[DEBUG LOG] There was a problem opening one of the extracted files. Operation aborted.")
            }

            return false
        }

        defer func() {
            err := rc.Close()
            if err != nil {
                if GetDebug() {
                    fmt.Println("[DEBUG LOG] There was a problem trying to close one of the extracted files. Operation aborted.")
                }

                return
            }
        } ()

        path := filepath.Join(loc, f.Name)

        if f.FileInfo().IsDir() {
            os.MkdirAll(path, f.Mode())
        } else {
            f, err := os.OpenFile(path, os.O_WRONLY | os.O_CREATE | os.O_TRUNC, f.Mode())
            if err != nil {
                if GetDebug() {
                    fmt.Println("[DEBUG LOG] There was a problem trying to create an empty file for writing data into. Operation aborted.")
                }

                return false
            }

            defer func() {
                err := f.Close()
                if err != nil {
                    if GetDebug() {
                        fmt.Println("[DEBUG LOG] There was a problem trying to close the written file. Operation aborted.")
                    }
                }
            } ()

            _, err = io.Copy(f, rc)
            if err != nil {
                if GetDebug() {
                    fmt.Println("[DEBUG LOG] There was a problem trying to copy data from the ZIP archive. Operation aborted.")
                }

                return false
            }
        }
        return true
    }

    for _, f := range descriptor.File {
        err := __ExtractAndWrite__(f)
        if !err {
            return false
        }
    }

    return true
}
