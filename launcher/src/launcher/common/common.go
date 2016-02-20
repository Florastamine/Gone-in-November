/* common.go */
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
package common

import (
    "runtime"
    "os"
    "io"
    "fmt"
)

const (
    GameExecutable string = "November"
    GameName       string = "Gone-in-November"
    FileName       string = "__version.lock"
    FilePath       string = "https://dl.dropboxusercontent.com/u/26857618/DP/"
)

var (
    DLDirectory    string = ""
    LockerPath     string = "./"
    LockerFileName string = ""
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

func GetDownloadDirectory() string {
    return DLDirectory
}

func SetDownloadDirectory(Directory string) {
    if Directory != "" {
        DLDirectory = Directory

        if GetDebug() {
            fmt.Println("[DEBUG LOG] Download directory selected at: ", GetDownloadDirectory())
        }
    }
}

func GetExecutableName() string {
    switch runtime.GOOS {
    case "windows":
        return GameExecutable + ".exe"
    }

    return GameExecutable
}

func CreateLocker() bool {
    if GetDebug() {
        fmt.Println("[DEBUG LOG] Creating the locker...")
    }

    locker, err := os.Create(GetLockerDirectory() + GetLockerFileName())
    if err != nil {
        return false
    }

    data := make([]byte, 1)
    _, err_write := locker.Write(data)
    if err_write != nil {
        return false
    }

    locker.Close()
    return true
}

func DestroyLocker() bool {
    if GetDebug() {
        fmt.Println("[DEBUG LOG] Releasing the locker...")
    }

    err := os.Remove(GetLockerDirectory() + GetLockerFileName())
    if err != nil {
        return false
    }

    return true
}

func SetLockerFileName(File string) {
    if File != "" {
        LockerFileName = File

        if GetDebug() {
            fmt.Println("[DEBUG LOG] Locker name changed to: ", GetLockerFileName())
        }
    }
}

func GetLockerFileName() string {
    return LockerFileName
}

func GetLockerDirectory() string {
    return LockerPath
}

func SetLockerDirectory(Directory string) {
    if Directory != "" {
        LockerPath = Directory

        if GetDebug() {
            fmt.Println("[DEBUG LOG] Locker directory selected at: ", GetLockerDirectory())
        }
    }
}

func IsExist(File string) bool {
    _, err := os.Stat(File) // It efficiently wraps the check for empty arguments, because err will yield nil anyway.
    if err != nil {
        if os.IsNotExist(err) {
            return false
        }
    }

    return true
}

func CopyFile(Dest, Src string) bool {
    in, err := os.Open(Src)
    if err != nil {
        if GetDebug() {
            fmt.Println("[DEBUG LOG] Cannot perform file copying: Cannot open the source file.")
        }

        return false
    }

    out, err := os.Create(Dest)
    if err != nil {
        if GetDebug() {
            fmt.Println("[DEBUG LOG] Cannot perform file copying: Cannot create the destination file.")
        }

        return false
    }

    _, err = io.Copy(out, in)
    if err != nil {
        if GetDebug() {
            fmt.Println("[DEBUG LOG] Cannot perform file copying: Cannot copy to the target file.")
        }

        return false
    }

    in.Close()
    err = out.Close()

    if err != nil {
        if GetDebug() {
            fmt.Println("[DEBUG LOG] Cannot perform file copying: Cannot \"finalize\" the target file after copying. The file may be corrupted.")
        }
    }

    return true
}
