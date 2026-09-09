cat > include/kasiski.h <<'EOF'
#ifndef KASISKI_H
#define KASISKI_H

#include <string>
#include <vector>
#include <map>

std::map<std::string, std::vector<int>>
find_repeated_patterns(
    const std::string& ciphertext,
    int length = 3
);

std::vector<int>
calculate_distances(
    const std::map<std::string, std::vector<int>>& patterns
);

std::vector<int> find_factors(int number);

int kasiski_analysis(const std::string& ciphertext);

#endif
EOF
