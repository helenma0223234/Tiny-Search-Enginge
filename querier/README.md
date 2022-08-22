TSE Querier by Yuchuan (Helen) Ma
DartID: F0043BC

In this lab you'll continue the Tiny Search Engine (TSE) by coding the Querier according to the [Requirements Spec](REQUIREMENTS.md).

## Functionality
 * this querier prints the set of documents that contain all the words in the query; you may treat operators ('and', 'or') as regular words.
 * this querier also supports 'and' and 'or' operators, but without precedence (in mathematical terms, it treats them as *left associative, equal precedence* operators).
 * this querier also supports 'and' precedence over 'or'.
 * this querier lso prints the document set in decreasing order by score, thus meeting the full specs.

## Sources and Citation
* the readlinep module is copied from the course's shared example folder, see less ~/cs50-dev/shared/readline.h for details.
* in the querier.c, the readNextWord function is partly sourced from the common cs50lib we were provided as the TSE project's starter/support kit.
