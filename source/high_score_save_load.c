#include "high_score_save_load.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 保存各难度最高分。
 */
void high_score_save(const int high_score[DIFFICULTY_COUNT]) {
	FILE* high_score_file = NULL;
	if (fopen_s(&high_score_file, HIGH_SCORE_FILE, "wb")) {
		fprintf(stderr, "Failed to open high score file when saving.\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DIFFICULTY_COUNT; ++i) {
		fprintf(high_score_file, "%d\n", high_score[i]);
		fprintf(stdout, "Saved: Difficulty %d has a high score of %d\n", i, high_score[i]);
	}

	fclose(high_score_file);
}

/**
 * @brief 读取各难度最高分。
 */
void high_score_load(int high_score[DIFFICULTY_COUNT]) {
	FILE* high_score_file = NULL;
	if (fopen_s(&high_score_file, HIGH_SCORE_FILE, "rb")) {
		fprintf(stderr, "Failed to open high score file when saving.\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DIFFICULTY_COUNT; ++i) {
		if (fscanf_s(high_score_file, "%d\n", high_score + i) != EOF) {
			fprintf(stdout, "Loaded: Difficulty %d has a high score of %d\n", i, high_score[i]);
		}
		else {
			high_score[i] = 0;
			fprintf(stderr, "EOF when loading high score.\n");
		}
	}

	fclose(high_score_file);
}
