#!/usr/bin/python3.10
"""
Developer: 2LT Jacob Schonhoff

Tool: TCFinder - (Type-Cast Finder)
USAGE: This tool can be used to check specified files or wildcard patterns for
       files to find all type-casts within C-language files. It will then check
       for comments above or in-line with the type-cast code-line. The tool then
       displays all uncommented type-casts.
REASON: To check for Barr-C Compliance with Barr-C 1.6.a
WARNING: Some multi-line single code-lines following Barr-C's 80 char/line limit
         may be listed as uncommented. A user should use this tool as a
         low-hanging fruit check rather than an end-all-be-all assessment.
"""

import re
import os
import glob
import argparse

def get_line_num(file, match):
    file.seek(0)
    line_num = 1
    for line_num, line in enumerate(file, start=1):
        if match in line:
            return line_num
    return None

def search_type_casts(files_to_check):
    type_casts = {}

    #First Capturing Group - Looks for comment above type-cast
    comment_above_pattern = r"(\/\/.*\s*.*|\*\/.*\s*.*)?"

    #Second Capturing Group - Looks for type-cast
    type_cast_pattern = r"(\(+\s*[a-zA-Z0-9_]+\s*\**\s*\)+"\
                         r"\s*\(*[a-zA-Z0-9_\-\>]+\)*;)"

    #Third Capturing Group - Looks for in-line comment
    inline_comment_pattern = r"(.*\/\/|.*\/\*)?"

    full_pattern = comment_above_pattern + type_cast_pattern +\
                   inline_comment_pattern
    pattern = re.compile(full_pattern, re.MULTILINE)
    for file_path in files_to_check:
        file_paths = glob.glob(file_path)
        matches = []
        for file in file_paths:
            with open(file, "r", encoding="utf-8") as opened_file:
                code = opened_file.read()
                for match in pattern.finditer(code):
                    #If neither of the comment-related capturing groups were
                    # matched, add to list
                    if match.group(1) is None and match.group(3) is None:
                        matches.append((match.group(2),
                                        get_line_num(opened_file,
                                                     match.group(2))))
            type_casts[file] = matches
    return type_casts

def parse_args():
    parser = argparse.ArgumentParser(description="Search specified files"\
                                                 "for Barr-C style type-casts.")
    parser.add_argument("files", nargs="+", help="List of file paths or"\
                                                 "wildcard pattern to search")
    return parser.parse_args()

def main():
    args = parse_args()
    found_type_casts = search_type_casts(args.files)
    print("Barr-C Style Type-Casts Found:")
    err_count = 0
    for file_path, matches in found_type_casts.items():
        for match, line_num in matches:
            print(f"{os.path.basename(file_path)}:{line_num}:{match}")
            err_count += 1
    print(f"\nTotal Detected Non-Commented Type-Casts: {err_count}\n")

if __name__ == "__main__":
    main()

#End of TCFinder Tool Code
