cat > include/frequency.h <<'EOF'
#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <string>
#include <vector>

double calculate_ic(const std::string& text);

std::vector<int>
frequency_analysis(const std::string& group);

int find_shift(const std::string& group);

std::vector<std::string>
split_into_groups(
    const std::string& ciphertext,
    int keyLength
);

#endif
EOF
