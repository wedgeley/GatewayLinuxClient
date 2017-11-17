#include <stdlib.h>

const size_t KEYCODE_LENGTH = 12;           // Any key codes longer than this will cause an error

const size_t KEYCODE_PAGE_SIZE = 4;         // Number of key codes to fetch at a time
                                            // ** Don't copy this value in production code as it is very inefficient
                                            //    to fetch small pages of keys.  A value of 100+ would be more reasonable

const int MAX_PAGES_TO_DISPLAY = 50;        // Never display more than this many pages
