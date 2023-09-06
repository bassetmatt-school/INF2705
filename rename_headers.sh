git_hpp () {
    for file in $(find . -name "*.h" -type f); do
        git mv "$file" "${file%.h}.hpp"
    done
}

git_h () {
    for file in $(find . -name "*.hpp" -type f); do
        git mv "$file" "${file%.hpp}.h"
    done
}

if [ "$1" == "hpp" ]; then
    git_hpp
elif [ "$1" == "h" ]; then
    git_h
else
    echo "Usage: $0 [hpp|h]"
fi
