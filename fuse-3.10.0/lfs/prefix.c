#include "prefix.h" /* prefix */

char *prefix = NULL;

void generate_prefix()
{
    long long prefix_len = 0;
    do
    {
        prefix_len += 10;
        logger(DEBUG, "Attempting to copy prefix into buffer of size %lld\n", prefix_len);
        if (prefix != NULL)
        {
            free(prefix);
        }
        prefix = malloc(prefix_len * sizeof(char));
        errno = 0;
        getcwd(prefix, prefix_len * sizeof(char));
        logger(DEBUG, "%s\n", strerror(errno));
    } while (errno == ERANGE);
    prefix_len = strlen(prefix);
    logger(DEBUG, "Started in directory: %s (%lld)\n", prefix, prefix_len);
}
