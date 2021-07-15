REM CONFIG_ATOMICS
gcc -D CONFIG_JSX=1 -D SHORT_OPCODES=1 -D CONFIG_BIGNUM=1 -E quickjs-opcode-gen.h -o quickjs-opcode-gen-out.h
node quickjs-opcode-gen.js
pause
