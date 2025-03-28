import os

def gather_code_files(root_dir, output_file='all_code.txt'):
    # List of file extensions we want to include
    valid_extensions = ('.cpp', '.h')
    
    with open(output_file, 'w', encoding='utf-8') as outfile:
        # Walk through all directories and files
        for root, dirs, files in os.walk(root_dir):
            # Skip .git directory if it exists
            if '.git' in dirs:
                dirs.remove('.git')
                
            for file in files:
                if file.endswith(valid_extensions):
                    file_path = os.path.join(root, file)
                    # Write a header for each file
                    outfile.write(f"\n\n=== File: {file_path} ===\n\n")
                    
                    try:
                        with open(file_path, 'r', encoding='utf-8') as infile:
                            outfile.write(infile.read())
                    except UnicodeDecodeError:
                        # Try a different encoding if UTF-8 fails
                        try:
                            with open(file_path, 'r', encoding='latin-1') as infile:
                                outfile.write(infile.read())
                        except Exception as e:
                            outfile.write(f"\n[ERROR reading file: {e}]\n")
                    except Exception as e:
                        outfile.write(f"\n[ERROR reading file: {e}]\n")

if __name__ == "__main__":
    # Start from the current directory
    base_dir = os.getcwd()
    print(f"Scanning directory: {base_dir}")
    gather_code_files(base_dir)
    print("All code has been compiled into 'all_code.txt'")