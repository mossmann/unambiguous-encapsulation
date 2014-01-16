// Example parser: delimited base64 file, syntax only.
//
// Demonstrates how to construct a Hammer parser that recognizes valid
// delimited base64 files.
//
// Note that no semantic evaluation of the sequence is performed, i.e. the
// byte sequence being represented is not returned, or determined. See
// base64_sem1.c and base64_sem2.c for examples how to attach appropriate
// semantic actions to the grammar.

#include "../src/hammer.h"

const HParser* document = NULL;

#define false 0
#define true 1

bool delim_base64_all_records_same_length(HParseResult *p)
{
    uint64_t i;
    uint64_t num_fields = 0;
    bool got_num_fields = false;
    uint64_t num_data_records = 0;
    unsigned int num_header_records = 0;

    if (p == NULL)
        return false;

    if (p->ast->token_type != TT_SEQUENCE)
        return false;

    if (p->ast->seq->used != 2)
        return false;

    const HParsedToken *data_records = p->ast->seq->elements[1];
    if (data_records->token_type != TT_SEQUENCE)
        return false;
    num_data_records = data_records->seq->used;

    printf("\n");

    for (i = 0; i < num_data_records; i++) {
        const HParsedToken *data_record =  data_records->seq->elements[i];
        if (data_record->token_type != TT_SEQUENCE)
            return false;
        if (!got_num_fields) {
            num_fields = data_record->seq->used;
            got_num_fields = true;
        }
        if (data_record->seq->used != num_fields) {
            printf("mismatched number of fields among data records\n");
            return false;
        }
    }

    // special case: single empty data record considered zero records
    if ((num_data_records == 1)
            && (data_records->seq->elements[0]->seq->used == 1)) {
        const HParsedToken *field = data_records->seq->elements[0]->seq->elements[0];
        if (field->token_type != TT_SEQUENCE)
            return false;
        if (field->seq->used != 2)
            return false;
        if (field->seq->elements[0]->token_type != TT_SEQUENCE)
            return false;
        if ((field->seq->elements[0]->seq->used == 0)
                && (field->seq->elements[1]->token_type == TT_NONE)) {
            // no base64 data present in only field of only data record
            num_fields = 0;
            got_num_fields = false;
            num_data_records = 0;
            printf("set num_data_records to zero due to single empty record\n");
        }
    }

    const HParsedToken *header_record = p->ast->seq->elements[0];
    if (header_record->token_type == TT_SEQUENCE) {
        num_header_records = 1;
        if (got_num_fields) {
            if (header_record->seq->used != num_fields) {
                printf("number of header fields doesn't match number of data fields\n");
                return false;
            }
        } else {
            num_fields = header_record->seq->used;
            got_num_fields = true;
        }
    } else if (header_record->token_type != TT_NONE) {
        return false;
    }

    printf("header records=%u\n", num_header_records);
    printf("data records=%lu\n", num_data_records);
    printf("fields=%lu\n", num_fields);

    return true;
}

void init_parser(void)
{
    // CORE
    const HParser *digit = h_ch_range(0x30, 0x39);
    const HParser *alpha = h_choice(h_ch_range(0x41, 0x5a), h_ch_range(0x61, 0x7a), NULL);

    // AUX.
    const HParser *plus = h_ch('+');
    const HParser *slash = h_ch('/');
    const HParser *equals = h_ch('=');

    const HParser *bsfdig = h_choice(alpha, digit, plus, slash, NULL);
    const HParser *bsfdig_4bit = h_in((uint8_t *)"AEIMQUYcgkosw048", 16);
    const HParser *bsfdig_2bit = h_in((uint8_t *)"AQgw", 4);
    const HParser *base64_3 = h_repeat_n(bsfdig, 4);
    const HParser *base64_2 = h_sequence(bsfdig, bsfdig, bsfdig_4bit, equals, NULL);
    const HParser *base64_1 = h_sequence(bsfdig, bsfdig_2bit, equals, equals, NULL);
    const HParser *base64 = h_sequence(h_many(base64_3),
                                       h_optional(h_choice(base64_2,
                                                           base64_1, NULL)),
                                       NULL);

    // delimiters
    const HParser *header_field_delim = h_ch(';');
    const HParser *header_record_delim = h_ch(':');
    const HParser *data_field_delim = h_ch(',');
    const HParser *data_record_delim = h_ch('.');

    const HParser *header_record = h_left(h_sepBy1(base64, header_field_delim),
            header_record_delim);
    const HParser *data_record = h_sepBy1(base64, data_field_delim);

    document = h_attr_bool(
            h_sequence(
                    h_optional(header_record),
                    h_sepBy1(data_record, data_record_delim),
                    h_end_p(),
                    NULL),
            delim_base64_all_records_same_length);
}


#include <stdio.h>

int main(int argc, char **argv)
{
    uint8_t input[102400];
    size_t inputsize;
    const HParseResult *result;

    init_parser();

    inputsize = fread(input, 1, sizeof(input), stdin);
    fprintf(stderr, "inputsize=%lu\ninput=", inputsize);
    fwrite(input, 1, inputsize, stderr);
    result = h_parse(document, input, inputsize);

    if(result) {
        fprintf(stderr, "parsed=%lld bytes\n", result->bit_length/8);
        h_pprint(stdout, result->ast, 0, 0);
        return 0;
    } else {
        return 1;
    }
}
