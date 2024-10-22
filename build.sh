reset

#Make the first argument all lowercase
platform="${1,,}"
#Show the help menu if needed
if [ "$platform" == "--help" ]
then
    echo "build.sh usage: ./build.sh [PLATFORM] or ./build.sh [PLATFORM] [CONFIGURATION PATH]"
    echo "Supported Platforms:"
    echo "  Windows"
    echo "  Linux"
    echo "  All (Windows and Linux)"
elif [ "$platform" == "" ]
then
    echo "Incorrect usage. Correct usage:"
    ./build.sh --help
else
    #Attempt to compile for the given platform
    echo "Attempting to compile for $1..."
    if [ "$platform" == "windows" ]
    then
        #Try to compile OneBar for Windows, either x86 or x64 depending on present libraries
        cmd_64=`command -v "x86_64-w64-mingw32-g++"`
        cmd_i686=`command -v "i686-w64-mingw32-g++"`
        if [ "$cmd_64" != "" ]
        then
            gpp="x86_64-w64-mingw32-g++"
        elif [ "$cmd_i686" != "" ]
        then
            gpp="i686-w64-mingw32-g++"
        fi
        if [ "$gpp" != "" ]
        then
            mkdir -p "bin/windows"
            cp onebar_config.json bin/windows/onebar_config.json
            $gpp src/main.cpp -o bin/windows/onebar.exe -Isrc/include -g -static-libstdc++ -static -fconcepts
            echo "Done."
        else
            echo "You must have MinGW installed to compile for Windows"
        fi
    elif [ "$platform" == "linux" ]
    then
        #Build OneBar for Linux, targetting Ubuntu or any debian linux-based OS
        mkdir -p "bin/linux"
        g++ src/main.cpp -Isrc/include -o bin/linux/onebar.o -fconcepts -g
        cp onebar_config.json bin/linux/onebar_config.json
        bin/linux/onebar.o "$2"
        echo "Done"
    elif [ "$platform" == "all" ]
    then
        ./build.sh windows
        ./build.sh linux
    else
        echo "${platform} is not a supported platform."
    fi
fi