/* copyright 2013 Sascha Kruse and contributors (see LICENSE for licensing information) */

#define _GNU_SOURCE
#include "markup.h"

#include <assert.h>
#include <stdbool.h>

#include "settings.h"
#include "utils.h"

static
char *markup_strip_tags(char *str)
{
        if (str == NULL) {
                return NULL;
        }

        string_strip_delimited(str, '<', '>');

        return str;
}

static
char *markup_unquote_entities(char *str)
{
        if (str == NULL) {
                return NULL;
        }

        str = string_replace_all("&quot;", "\"", str);
        str = string_replace_all("&apos;", "'", str);
        str = string_replace_all("&amp;", "&", str);
        str = string_replace_all("&lt;", "<", str);
        str = string_replace_all("&gt;", ">", str);

        return str;
}

static
char *markup_quote_entities(char *str)
{
        if (str == NULL) {
                return NULL;
        }

        str = string_replace_all("&", "&amp;", str);
        str = string_replace_all("\"", "&quot;", str);
        str = string_replace_all("'", "&apos;", str);
        str = string_replace_all("<", "&lt;", str);
        str = string_replace_all(">", "&gt;", str);

        return str;
}

static
char *markup_br2nl(char *str)
{
        if (str == NULL) {
                return NULL;
        }

        str = string_replace_all("<br>", "\n", str);
        str = string_replace_all("<br/>", "\n", str);
        str = string_replace_all("<br />", "\n", str);
        return str;
}

/*
 * Strip any markup from text
 */
char *markup_strip(char *str)
{
        if (str == NULL) {
                return NULL;
        }

        str = markup_strip_tags(str);

        /* We want to quote; but unquote first to avoid
         * double-escaping anything already quoted. */
        str = markup_unquote_entities(str);
        str = markup_quote_entities(str);

        return str;
}

/*
 * Transform the string in accordance with `markup_mode` and
 * `settings.ignore_newline`
 */
char *markup_transform(char *str, enum markup_mode markup_mode)
{
        if (str == NULL) {
                return NULL;
        }

        switch (markup_mode) {
        case MARKUP_NULL:
                assert(false);
                break;
        case MARKUP_NO:
                str = markup_quote_entities(str);
                break;
        case MARKUP_STRIP:
                str = markup_br2nl(str);
                str = markup_strip(str);
                break;
        case MARKUP_FULL:
                str = markup_br2nl(str);
                break;
        }

        if (settings.ignore_newline) {
                str = string_replace_all("\n", " ", str);
        }

        return str;
}

/* vim: set tabstop=8 shiftwidth=8 expandtab textwidth=0: */
