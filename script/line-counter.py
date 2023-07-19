from pathlib import Path
import sys

excluded_dirs = ('.devcontainer', '.git', '.vscode', 'bin', 'build', 'data', 'dependency', 'dependencies')
included_exts = ('.cpp', '.hpp', '.h', '.c', '.cu', '.cuh', '.glsl', '.cmake', '.py', '.sh', '.md', '.json', 'CMakeLists.txt')

def count_lines_in_file(file_name: Path) -> int:
    """
    Count the number of lines in a file.
    Args:
        file_name (Path): The name of the file.
    Returns:
        (int): The number of lines in the file.
    """

    with open(file_name, 'r', encoding='utf-8') as f:
        lines = len(f.readlines())
    return lines

def count_lines(path: Path) -> tuple[int, int]:
    """
    Recursively count the number of files and lines in a directory.
    Args:
        path (Path): The path of the directory.
    Returns:
        (tuple[int, int]): The number of files and lines in the directory.
    """

    total_files, total_lines = 0, 0
    for entry in path.iterdir():
        # Omitting the files that are in excluded directories.
        if entry.name in excluded_dirs:
            continue
        # Recursively counting files in each subdirectories.
        if entry.is_dir():
            files, lines = count_lines(entry)
            total_files += files
            total_lines += lines
        # Counting files.
        if entry.is_file():
            if entry.name.endswith(included_exts):
                lines = count_lines_in_file(entry)
                print(entry, lines)
                total_files += 1
                total_lines += lines
    return total_files, total_lines

def main():
    if len(sys.argv) != 2:
        print("Usage: python {} <path>".format(sys.argv[0]))
        return
    print("Number of files {}, number of lines {}.".format(*count_lines(Path(sys.argv[1]))))

if __name__ == '__main__':
    main()
