char *
string_space_skip(char *string)
{
    if (!string) {
        return NULL;
    } else {
        while (*string && dp_isspace(*string)) {
            string++;
        }

        return string;
    }
}

