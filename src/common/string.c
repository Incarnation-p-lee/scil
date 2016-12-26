char *
string_space_skip(char *string)
{
    if (!string) {
        return NULL;
    } else {
        while (dp_isspace(*string)) {
            string++;
        }

        return string;
    }
}

char *
string_space_skip_back(char *string)
{
    if (!string) {
        return NULL;
    } else {
        while (dp_isspace(*string)) {
            string--;
        }

        return string;
    }
}

