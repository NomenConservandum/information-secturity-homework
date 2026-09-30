import sys
import os
from pathlib import Path


def encode_message(path: Path, message: str) -> int:
    """Read file and print its content (encode stub)."""
    init_file_content = ""
    with open(path, 'r', encoding='cp1251') as f:
        for line in f:
            init_file_content += line.rstrip('\n')  # getline drops the '\n'
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


if __name__ == "__main__":
    sys.exit(main(sys.argv))