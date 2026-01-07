#if !defined(DEFS_H)
#define DEFS_H

#define GDF_UNUSED(x) ((void)x)

#define TRY_BOOLEAN(path, out)                               \
    {                                                        \
        toml_datum_t datum = toml_seek(result.toptab, path); \
        if (datum.type == TOML_BOOLEAN) {                    \
            out = datum.u.boolean;                           \
        }                                                    \
    }
#define TRY_STRING(path, out)                                \
    {                                                        \
        toml_datum_t datum = toml_seek(result.toptab, path); \
        if (datum.type == TOML_STRING) {                     \
            out = datum.u.s;                                 \
        }                                                    \
    }

#endif // DEFS_H
