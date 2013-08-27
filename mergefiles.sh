for filename in ./src/*.cpp; do
    # each time through the loop, ${filename} will hold the name
    # of the next *.txt file.  You can then arbitrarily process
    # each file
    cat "${filename}"
    echo

# You can add redirection after the done (which ends the
# for loop).  Any output within the for loop will be sent to
# the redirection specified here
done > ./CompiledSource/main.cpp