import sys
from pathlib import Path

def letter_substitution(char: str) -> str:
    # а е о р с у х А B Е К О Р С Т Х
    match char:
        case 'а':
            return 'a'
        case 'е':
            return 'e'
        case 'о':
            return 'o'
        case 'р':
            return 'p'
        case 'с':
            return 'c'
        case 'у':
            return 'y'
        case 'х':
            return 'x'
        case 'А':
            return 'A'
        case 'В':
            return 'B'
        case 'Е':
            return 'E'
        case 'К':
            return 'K'
        case 'О':
            return 'O'
        case 'Р':
            return 'P'
        case 'С':
            return 'C'
        case 'Т':
            return 'T'
        case 'Х':
            return 'X'

def encode_message(path: Path, message: str) -> int:
    """Read file and print its content (encode stub)."""
    init_file_content = ""
    with open(path, 'r', encoding='cp1251') as f:
        for line in f:
            init_file_content += line
    
    bitMessage = bin(int.from_bytes(message.encode(encoding='cp1251'), 'big'))[2:]
    print(f"Binary representation of the message: {bitMessage}")

    while (bitMessage):
        bit = bitMessage[len(bitMessage) - 1]
        bitMessage = bitMessage[:len(bitMessage) - 1]
        if (bit == '1'):
            init_file_content.find()
        
    print(init_file_content)
    return 0


def decode_message(path: Path) -> int:
    """Read file and print its content (decode stub)."""
    init_file_content = ""
    with open(path, 'r', encoding='cp1251') as f:
        for line in f:
            init_file_content += line.rstrip('\n')
    print(init_file_content)
    return 0

# E.g. python3 task2.py /home/nomen/Documents/sth.txt 2
def main(argv: list[str]) -> int:
    arguments_required = "1st - path to the file;\n2nd - mode (1 for encode and 2 for decode)\n3rd - message to encode (if mode 1 is chosen)"
    

    # argv[0] is the program name, so we look at argv[1:]
    argc = len(argv)

    if (
        argc < 3
        or argc > 4
        or (argv[2] == '1' and argc != 4)
        or (argv[2] == '2' and argc != 3)
    ):
        print(f"WRONG NUMBER OF ARGUMENTS ({argc - 1})!\n{arguments_required}",
              file=sys.stderr)
        return 1

    file_path = Path(argv[1])

    if not file_path.exists():
        print(f"NO SUCH FILE: {file_path}!", file=sys.stderr)
        return 1

    # Branching on mode
    if argv[2] == '1':
        encode_message(file_path, argv[3])
    elif argv[2] == '2':
        decode_message(file_path)
    else:
        print(f"WRONG MODE! The arguments needed:\n{arguments_required}",
              file=sys.stderr)
        return 1

    return 0

main(sys.argv)