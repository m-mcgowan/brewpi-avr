prev=$1
next=$2
# produce a file contaning symbol names, a tab and the size 
# - convert line endings from windows, 
# - extract size and symbol fields. while spaces are used to delineate columns,
# spaces can also appear in symbol names, so these are extracted by position.
# $1 - the filename to prepare
sizes() 
{
  cat $1 | tr -d '\r' | sed -e 's/^.\{10\}\([0-9]*\).\(.*\)/\2\t\1/'| sort
}

cut_symbol() 
{
  sizes $1 | cut -f1 
}

all_symbols() 
{
  cat <(cut_symbol $prev) <(cut_symbol $next) | uniq | sed -e 's/$/\t00000000/' 
}


# sum two files containing symbols and a size
sum()
{
   cat $1 $2  | awk -F'\t' ' 
 {a[$1]+=$2} 
 END {for(i in a) print i "\t" a[i]} 
' | sort "-t	" -k2 -n -r
}

negate()
{
   sed -e 's/\(.*\t\)\(.*\)/\1-\2/' $1
}
normalize()
{
   sum <(all_symbols) <(sizes $1)
}

filter_and_sum()
{
  awk -F'\t' '{S+=$2; if ($2!=0) print} END {print "total\t",S}' 
}
# for normalize each file to include the size for all symbols,
# (symbols not present have size 0)
# 

# join both normalized files to get the size before and after
# use awk to subtract one column from the other, output those that are non zero
 
# comm -13 <(prep $1) <(prep $2)


sum <(normalize $2) <(negate <(normalize $1)) | filter_and_sum
#sum <(negate <(normalize $1)) <(normalize $1)
#negate <(normalize $1)
