#!/bin/sh
# This script inserts GNU V2 license info at the top of the source files.
#
# Currently this script inserts new license info with no regard for any
# license info already inserted by running this script.
# TODO: Alter script to remove any license info from the top of the source file
# before inserting a second time.

source_dir='../src'
(find "$source_dir" -name *.c; find "$source_dir" -name *.h) \
| while read file_path; do
    file_name=$(basename "$file_path")

    new=$(cat << TEMPLATE
/*  This file ($file_name) is part of the ta application.
    Copyright (C) 2016-2017 Aaron Calder

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
TEMPLATE
    cat "$file_path")

    echo "$new" > "$file_path"
done
