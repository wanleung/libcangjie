#include "util.h"

/* These are some simple helper functions, used by various parts of libcangjie.
 *
 * They are not (and should not) be exposed as part of the public API.
 */

// This could eventually be replaced by boost::starts_with
bool startswith(std::string s, std::string begin) {
    return (s.find(begin) == 0);
}

// This could eventually be replaced by boost::ends_with
bool endswith(std::string s, std::string ending) {
    if (s.length() >= ending.length()) {
        return (s.compare(s.length() - ending.length(), ending.length(), ending) == 0);
    } else {
        return false;
    }
}
