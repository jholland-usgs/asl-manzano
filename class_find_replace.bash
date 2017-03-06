# There is a specific format to class names in this project:

# .h headers have include guards with CLASS_NAME format
# .cpp files in auto generation need also to be changed so that they write .h
# .cpp and .h files have names with class_name format
# inside the files, classes have ClassName format

# example of format for this program:
# this="Class_Name"
# that="Class_Name"

this="Multi_Command"
that="Command_Container"

header_this="$(echo $this | tr /a-z/ /A-Z/)"
header_that="$(echo $that | tr /a-z/ /A-Z/)"

class_this=${this//_/}
class_that=${that//_/}

file_this="$(echo $this | tr /A-Z/ /a-z/)"
file_that="$(echo $that | tr /A-Z/ /a-z/)"

echo $header_this
echo $class_this
echo $file_this

# change inside the files:
find . -type f -name '*.h'     -exec sed -i '' s/$header_this/$header_that/g {} +
find . -type f -name '*.cpp'   -exec sed -i '' s/$header_this/$header_that/g {} +

find . -type f -name '*.h'     -exec sed -i '' s/$class_this/$class_that/g {} +
find . -type f -name '*.cpp'   -exec sed -i '' s/$class_this/$class_that/g {} +

find . -type f -name '*.h'     -exec sed -i '' s/$file_this/$file_that/g {} +
find . -type f -name '*.cpp'   -exec sed -i '' s/$file_this/$file_that/g {} +

find . -type f -name '*.cmake' -exec sed -i '' s/$file_this/$file_that/g {} +

# change the names of the files
find . -type f -name '*.cpp'   -exec rename "s/$file_this/$file_that/" {} ";"
find . -type f -name '*.h'     -exec rename "s/$file_this/$file_that/" {} ";"
