# There is a specific format to class names in this project:

# .h headers have include guards with CLASS_NAME format
# .cpp and .h files have names with class_name format
# inside the files, classes have ClassName format

this="Multi_Command_Map"
that="Command_Map_Ni"

file_this="$(echo $this | tr /A-Z/ /a-z/)"
file_that="$(echo $that | tr /A-Z/ /a-z/)"

header_this="$(echo $this | tr /a-z/ /A-Z/)"
header_that="$(echo $that | tr /a-z/ /A-Z/)"

class_this=${this//_/}
class_that=${that//_/}

echo $header_this
echo $file_this
echo $class_this

# change inside the files:
#find . -type f -name '*.h' -o -name '*.cpp' -exec sed -i '' s/MultiCommand/CommandContainer/ {} +
