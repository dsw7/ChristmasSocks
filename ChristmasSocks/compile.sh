LOCALPATH=$(dirname $0)

generate_makefiles() {
    cmake -S ${LOCALPATH} -B ${LOCALPATH}/bin
}

compile_binary_from_makefiles() {
    make --jobs=12 -C ${LOCALPATH}/bin
}

generate_makefiles
compile_binary_from_makefiles
