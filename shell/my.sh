#!/usr/bin/bash

fun()
{
    my=hello
    echo "my=$my"
}
fun
echo "my=$my"

a=1
let "$a +=1"
exho "a=$a"

exit 0
