#!/usr/bin/env bash

# Directory to process (defaults to current directory if not provided)
TARGET_DIR="${1:-.}"

# Current date in YYYY-MM-DD format
CURRENT_DATE=$(date +%Y-%m-%d)
AUTHOR_NAME="Alan Ksenzov"

# Process all regular files in the directory
for filepath in "$TARGET_DIR"/*; do
    # Skip if not a regular file
    [[ -f "$filepath" ]] || continue

    filename=$(basename "$filepath")
    extension="${filename##*.}"

    # Determine comment style based on extension
    case "$extension" in
        sh|py|rb|pl|yaml|yml|r|dockerfile|Dockerfile)
            comment_prefix="#"
            ;;
        c|cpp|h|hpp|js|ts|java|cs|go|rs|swift|kt|php|css|scss)
            comment_prefix="//"
            ;;
        *)
            # Skip unsupported file types
            continue
            ;;
    esac

    # Construct 6-line comment header
    header="${comment_prefix}\n${comment_prefix}\n${comment_prefix}      ${filename}\n${comment_prefix}      ${AUTHOR_NAME} ${CURRENT_DATE}\n${comment_prefix}\n${comment_prefix}\n"

    # Prepend header to file safely via a temp file
    temp_file=$(mktemp)
    printf "%b\n" "$header" | cat - "$filepath" > "$temp_file" && mv "$temp_file" "$filepath"

    echo "Updated: $filename"
done
