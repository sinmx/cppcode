Tbl = {"This is line 4", "This line is out of place", "This is line 2"}
io.write(table.concat(Tbl, "\t"), "\n")
table.insert(Tbl, "This is line 1")
io.write(table.concat(Tbl, "\t"), "\n")
table.remove(Tbl, 2)
io.write(table.concat(Tbl, "\t"), "\n")
table.insert(Tbl, "This is line 3")
io.write(table.concat(Tbl, "\t"), "\n")
table.insert(Tbl, "This is line " .. table.maxn(Tbl) + 1)
io.write(table.concat(Tbl, "\t"), "\n")
table.sort(Tbl)
io.write(table.concat(Tbl, "\t"), "\n")
