MOVE="mv"

to_hpp () {
    for file in $(find . -name "*.h" -type f); do
        $MOVE "$file" "${file%.h}.hpp"
    done
    for file in $(find . -name "*.cpp" -o -name "*.hpp" -type f); do
        sed -i "s|.h\"|.hpp\"|g" $file
        sed -i "s|<SDL.h>|<SDL2/SDL.h>|g" $file
    done
}

to_h () {
    for file in $(find . -name "*.hpp" -type f); do
        $MOVE "$file" "${file%.hpp}.h"
    done
    for file in $(find . -name "*.cpp" -o -name "*.h" -type f); do
        sed -i "s|.hpp\"|.h\"|g" $file
        sed -i "s|<SDL2/SDL.h>|<SDL.h>|g" $file
    done
}

if [ "$1" == "hpp" ]; then
    to_hpp
elif [ "$1" == "h" ]; then
    to_h
else
    echo "Usage: $0 [hpp|h]"
fi
