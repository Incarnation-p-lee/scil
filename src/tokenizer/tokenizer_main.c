s_token_t *
tokenizer_process_file(char *filename)
{
    s_tokenizer_aim_t *aim;

    if (filename) {
        aim = tokenizer_aim_open(filename);
        return (void *)aim;
    } else {
        scil_log_print_and_exit("Attempt to access NULL pointer.\n");
        return NULL;
    }
}

