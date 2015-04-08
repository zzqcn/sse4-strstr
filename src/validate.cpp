#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <string>
#include <vector>

#include <smmintrin.h>

// ------------------------------------------------------------------------

#include <utils/sse.cpp>
#include <utils/bits.cpp>
#include "sse4-strstr.cpp"

// ------------------------------------------------------------------------

#include <utils/ansi.cpp>
#include "application_base.cpp"


class Application final: public ApplicationBase {

public:
    bool run() {

        const auto n = words.size();

        for (size_t i = 0; i < n; i++) {

            const auto& word = words[i];

            const auto reference = file.find(word);
            const auto result = sse4_strstr(file, word);
    
            if (i % 100 == 0) {
                print_progress(i, n);
            }

            if (reference != result) {
                putchar('\n');
                const auto msg = ansi::seq("ERROR", ansi::RED);
                printf("%s: std::find result = %lu, sse4_string = %lu\n",
                    msg.data(), reference, result);

                printf("word: '%s' (length %lu)\n", word.data(), word.size());

                return false;
            }
        }

        print_progress(n, n);

        putchar('\n');

        const auto msg = ansi::seq("OK", ansi::GREEN);
        printf("%s\n", msg.c_str());

        return true;
    }


    void print_help(const char* progname) {
        std::printf("usage: %s [file] [words]\n", progname);
        std::puts("");
        std::puts("Search all words in a file using std::string::find and SSE4 procedure");
        std::puts("");
        std::puts("Parameters:");
        std::puts("");
        std::puts("  file  - arbitrary file");
        std::puts("  words - list of words in separate lines");
    }

private:
    void print_progress(size_t pos, size_t n) {

        printf("validating... %0.2f%% (%lu/%lu)\r", 100.0*pos/n, pos, n);
        fflush(stdout);
    }
};


int main(int argc, char* argv[]) {

    Application app;

    if (argc == 3) {
        try {
            app.prepare(argv[1], argv[2]);

            const auto ret = app.run();

            return ret ? EXIT_SUCCESS : EXIT_FAILURE;
        } catch (ApplicationBase::Error& err) {

            const auto msg = ansi::seq("Error: ", ansi::RED);
            printf("%s: %s\n", msg.data(), err.message.data());

            return EXIT_FAILURE;
        }
    } else {
        app.print_help(argv[0]);

        return EXIT_FAILURE;
    }
}