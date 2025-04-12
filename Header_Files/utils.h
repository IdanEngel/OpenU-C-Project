/* util.h */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>  /* for size_t */

/**
 * @brief Safely allocates memory and exits on failure.
 *
 * @param size Size in bytes to allocate.
 * @return Pointer to allocated memory, never NULL.
 */
void *safe_malloc(size_t size);

/**
 * @brief Adds a new file extension to a file name (e.g., "prog1.as" → "prog1.am").
 *
 * This function allocates a new string with the modified name.
 * The caller is responsible for freeing the returned pointer.
 *
 * @param file_name The base file name (may include extension).
 * @param new_ext The new extension to add, e.g. ".am"
 * @return A new string with the updated name.
 */
char *add_extension(const char *file_name, const char *new_ext);

#endif /* UTILS_H */
