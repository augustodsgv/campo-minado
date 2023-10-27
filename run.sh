clear
if ! [ -f "$(pwd)/campo_minado" ]; then
    echo "compilando..."
    gcc campo_minado.c -o campo_minado -lm
fi
./campo_minado