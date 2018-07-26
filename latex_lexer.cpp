#include <iostream>

enum class CharacterType
{
    SPACE,
    LETTER,
    NUMBER,
    BS,
    OTHER
};

CharacterType classify(char c)
{
    if (c == '\\')
    {
        return CharacterType::BS;
    }
    else if (c == ' ')
    {
        return CharacterType::SPACE;
    }
    else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        return CharacterType::LETTER;
    }
    else if (c >= '0' && c <= '9')
    {
        return CharacterType::NUMBER;
    }
    else
    {
        return CharacterType::OTHER;
    }
}

enum class LexerState
{
    AWAITING_TOKEN,
    READING_NUMBER,
    READING_COMMAND
};

// returns length of token
// marker is updated to start of token
int next_token(const char* latex, int* start)
{
    int marker = *start;
    LexerState state = LexerState::AWAITING_TOKEN;
    while (latex[marker])
    {
        CharacterType char_class = classify(latex[marker]);

        switch (state)
        {
        case LexerState::AWAITING_TOKEN:
            *start = marker;    // update start to current position
            if (char_class == CharacterType::BS)
            {
                state = LexerState::READING_COMMAND;
            }
            else if (char_class == CharacterType::NUMBER)
            {
                state = LexerState::READING_NUMBER;
            }
            else if (char_class != CharacterType::SPACE)
            {
                // single charager token
                return 1;
            }
            // otherwise we have an invalid token character, which will be ignored
            break;

        case LexerState::READING_COMMAND:
            if (char_class != CharacterType::LETTER)
            {
                // note that a multi-character token can only be composed of letters
                return marker - *start;
            }
            // otherwise keep looking for token end
            break;


        case LexerState::READING_NUMBER:
            if (char_class != CharacterType::NUMBER)
            {
                // note that a multi-character token can only be composed of letters
                return marker - *start;
            }
            // otherwise keep looking for token end
            break;
        }
        
        marker++;
    }

    // we hit the end of the string
    return marker - *start;
}
