from pathlib import Path
import sys

def check_include_guard(root: Path):
    """Check the include guard of a header file."""
    for path in root.rglob("*.hpp"):
        expected_guard = path.relative_to(root) \
            .as_posix() \
            .upper() \
            .replace("/", "_") \
            .replace(".", "_") \
            .replace("-", "_")
        expected_guard = f"_{expected_guard}_"
        with open(path, "r") as f:
            content = f.readlines()
        if len(content) < 3:
            continue
        firstline = content[0]
        if firstline != f"#ifndef {expected_guard}\n":
            content[0] = f"#ifndef {expected_guard}\n"
            content[1] = f"#define {expected_guard}\n"
            content[-1] = f"#endif // !{expected_guard}\n"
            with open(path, "w") as f:
                f.writelines(content)
            print(f"Fixed include guard of {path}")

if __name__ == "__main__":
    root = sys.argv[1]
    check_include_guard(Path(root))
