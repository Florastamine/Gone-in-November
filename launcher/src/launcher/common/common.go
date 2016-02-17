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
)

const (
    GameExecutable string = "November"
    GameName       string = "Gone-in-November"
    FileName       string = "__version.lock"
    FilePath       string = "https://dl.dropboxusercontent.com/u/26857618/DP/"
)

var (
    DLDirectory    string = ""
)

func GetDownloadDirectory() string {
    return DLDirectory
}

func SetDownloadDirectory(Directory string) {
    if Directory != "" {
        DLDirectory = Directory
    }
}

func GetExecutableName() string {
    switch runtime.GOOS {
    case "windows":
        return GameExecutable + ".exe"
    }

    return GameExecutable
}
