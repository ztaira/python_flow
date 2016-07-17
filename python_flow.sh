#!/bin/bash

scan_function() {
    local function_name="$1"
    local indent_level="$2"
    local line=$(printf "%0.s " $(seq 1 $indent_level))
    line="${line}def ${function_name}"
}

pyflow() {
    local file_name="$1"
    local start=$(grep -n 'if __name__ == .__main__.:' $file_name)
    local end=$(grep -n '.' $file_name | tail -n 1)
    echo "Start:"
    echo $start
    echo "End:"
    echo $end
}

