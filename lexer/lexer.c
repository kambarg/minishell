#include <../includes/lexer.h>
// #include "../minishell.h" //// Only if needed for global structures/macros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // is alnum, is_space

// Function to check if a character is a special operator
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// aka ctype.h lib function isspace
int	is_space(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

// Expands environment variables in any token
char *expand_env_variables(const char *input) {
    size_t len = strlen(input);
    char *expanded = malloc(len * 2 + 1); // Extra space for large expansions
    if (!expanded) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    size_t i = 0, j = 0;
    while (input[i]) {
        if (input[i] == '$' && (isalpha(input[i + 1]) || input[i + 1] == '_')) {
            // Start extracting variable name
            size_t var_start = i + 1;
            while (isalnum(input[var_start]) || input[var_start] == '_') var_start++;

            // Extract variable name
            size_t var_len = var_start - (i + 1);
            char var_name[var_len + 1];
            strncpy(var_name, &input[i + 1], var_len);
            var_name[var_len] = '\0';

            // Get environment variable value
            char *var_value = getenv(var_name);
            if (var_value) {
                strcpy(&expanded[j], var_value);
                j += strlen(var_value);
            }
            i = var_start; // Move past variable name
        } else {
            expanded[j++] = input[i++];
        }
    }

    expanded[j] = '\0';
    return expanded;
}

char *extract_quoted_token(const char *input, size_t *index) {
    char quote = input[*index]; // Store opening quote (' or ")
    (*index)++; // Move past the quote

    size_t start = *index;
    while (input[*index] && input[*index] != quote) {
        (*index)++;
    }

    if (input[*index] == '\0') { // Error: Missing closing quote
        fprintf(stderr, "Error: Missing closing quote\n");
        return NULL;
    }

    size_t length = *index - start;
    (*index)++; // Move past the closing quote

    char *token = malloc(length + 1);
    if (!token) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }
    strncpy(token, input + start, length);
    token[length] = '\0';

    // Expand variables **only inside double quotes**
    if (quote == '"') {
        char *expanded_token = expand_env_variables(token);
        free(token);
        return expanded_token;
    }

    return token; // Return unchanged if inside single quotes
}

char **tokenize(char *input) {
    if (!input) return NULL;

    size_t input_len = strlen(input);
    char **tokens = malloc((input_len + 1) * sizeof(char *));
    if (!tokens) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    size_t i = 0, token_count = 0;
    while (i < input_len) {
        while (i < input_len && isspace(input[i])) i++; // Skip spaces

        if (i >= input_len) break;

        size_t start = i;

        // Handle quoted tokens
        if (input[i] == '"' || input[i] == '\'') {
            char *quoted_token = extract_quoted_token(input, &i);
            if (!quoted_token) return NULL;
            tokens[token_count++] = quoted_token;
        }
        // Handle operators (|, <, >, >>, <<) as separate tokens
        else if (is_operator(input[i])) {
            size_t op_len = 1;
            if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i]) {
                op_len = 2; // Handle ">>" or "<<"
            }

            tokens[token_count] = malloc(op_len + 1);
            if (!tokens[token_count]) return NULL;
            strncpy(tokens[token_count], &input[i], op_len);
            tokens[token_count][op_len] = '\0';
            token_count++;
            i += op_len;
        }
        // Handle normal words (expand variables)
        else {
            while (i < input_len && !isspace(input[i]) && !is_operator(input[i])) {
                i++;
            }
            size_t token_length = i - start;
            char *raw_token = malloc(token_length + 1);
            if (!raw_token) return NULL;
            strncpy(raw_token, &input[start], token_length);
            raw_token[token_length] = '\0';

            // Expand variables in unquoted tokens
            char *expanded_token = expand_env_variables(raw_token);
            free(raw_token);
            tokens[token_count++] = expanded_token;
        }
    }

    tokens[token_count] = NULL;
    return tokens;
}

// Test function prints tokens
// char input[] = "echo  "Hello    world" > file.txt | cat < input.txt";
int	lexer(char *input)
{
	char	**tokens;
	int		i;

	tokens = tokenize(input);
	i = 0;
	while (tokens[i])
	{
		printf("Token[%d]: %s\n", i, tokens[i]);
		free(tokens[i]);
		i++;
	}
	free(tokens);
	return (0);
}
