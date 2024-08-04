//please implement a class in c language, make sure no errors or warnings : the compiler is gcc.the flags are - pedantic - -ansi - Wall.
//implememt a function  "macro_table_builder".
// the function return an array of strings.
//you need to hold in the class helper dictionary named "macro_defintions" to hold macro defintions.the key is the macro name (string), the value is a matrix of strings.each line(row) is a set of macro commands (just strings).
//you need to hold in the class a boolean flag, named "is_macro_context".
//you need to hold in the class a string called "current_macro_name".
//the function get an array of strings:
//if the string in the first cell exists in the "macro_defintions" dictionary, return the matrix which is the value of the key from the "macro_defintions".
//if the "is_macro_context" is on, add the the cells to the "macro_defintions" as  a new row to the matrix which is the value of the key "current_macro_name". return NULL.
//if the first cell is "macr", switch on the "is_macro_context" flag on, set the "current_macro_name" to the string in the second cell.add an item to "macro_defintions", the key is the "current_macro_name".the value is an empty matrix of strings. return NULL.
//if the first cell is "endmacr", set "is_macro_context" to false. return NULL.
// retutn the array of strings the function got as parameter.
