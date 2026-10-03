import sys
from pathlib import Path

target_letters = {'а', 'е', 'о', 'р', 'с', 'у', 'х', 'А', 'В', 'Е', 'К', 'О', 'Р', 'С', 'Т', 'Х'}

substitution_map = {
    'а': 'a', 'е': 'e', 'о': 'o', 'р': 'p', 'с': 'c', 'у': 'y', 'х': 'x',
    'А': 'A', 'В': 'B', 'Е': 'E', 'К': 'K', 'О': 'O', 'Р': 'P', 'С': 'C', 'Т': 'T', 'Х': 'X',
}

def is_substitutable(char: str) -> bool:
    return char in target_letters or char in substitution_map.values()

def letter_substitution(char: str) -> str:
    if char in substitution_map:
        return substitution_map[char]
    else: # non-target letter, it can be an already substituted one
        return '-'

def decode_message(path: Path) -> int:
    init_file_content = ""
    with open(path, 'r', encoding='cp1251') as f:
        for line in f:
            init_file_content += line

    bitMessage = ""
    for l in init_file_content:
        if (is_substitutable(l)):
            if letter_substitution(l) == '-': # non Russian letter - it was substituted
                bitMessage = '1' + bitMessage
            else: # original Russian letter
                bitMessage = '0' + bitMessage
        if len(bitMessage) % 8 == 0 and bitMessage[:8] == '0' * 8: # encountered first zero byte, the message has ended
            bitMessage = bitMessage[8:]
            break

    # print(f"Binary representation of the message: {bitMessage}")

    message_bytes = bytearray()
    for i in range(0, len(bitMessage), 8):
        byte_chunk = bitMessage[i:i+8]
        byte_val = int(byte_chunk, 2)
        message_bytes.append(byte_val)
        
    decoded_message = message_bytes.decode(encoding='cp1251')

    print(f"Decoded message: {decoded_message}")
    
    return 0

def main(argv: list[str]) -> int:
    arguments_required = "1st - path to the file"
    
    argc = len(argv)
    if argc != 2:
        print(f"WRONG NUMBER OF ARGUMENTS ({argc - 1})!\n{arguments_required}", file=sys.stderr)
        return 1

    file_path = Path(argv[1])
    if not file_path.exists():
        print(f"NO SUCH FILE: {file_path}!", file=sys.stderr)
        return 1

    decode_message(file_path)
    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))
