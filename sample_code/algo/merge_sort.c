// release: gcc -O3 -Wall -o ./merge_sort ./merge_sort.c
// debug  : gcc -g -Wall -o ./merge_sort_debug ./merge_sort.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

typedef int32_t key_t;
typedef int32_t value_t;

typedef struct  {
	key_t   key;
	value_t value;
} tuple_t;

void merge(tuple_t *a, int left, int mid, int right, tuple_t *tmp) {
	int i = left;
	int j = mid;
	int k = left;

	while (i < mid && j < right) {
		if (a[i].key < a[j].key)
			tmp[k++] = a[i++];
		else
			tmp[k++] = a[j++];
	}

	while (i < mid)
		tmp[k++] = a[i++];

	while (j < right)
		tmp[k++] = a[j++];

//	memcpy(a + left, tmp + left, sizeof(tuple_t) * (right - left));
}

// recursive
void merge_sort(tuple_t *a, int left, int right, tuple_t *tmp) {
	int mid;
	if (left < right - 1) {
		mid = (right + left) >> 1;
		merge_sort(a, left, mid, tmp);
		merge_sort(a, mid, right, tmp);
		merge(a, left, mid, right, tmp);
		memcpy(a + left, tmp + left, sizeof(tuple_t) * (right - left));
	}
}

// non-recursive
void merge_sort2(tuple_t *a, int len, tuple_t *tmp) {
	if (len <= 1)
		return;

	int toggle = 0;
	tuple_t *src, *dst;
	for (int width = 1; width < len; width <<= 1) {
		if (toggle & 1) {
			src = tmp;
			dst = a;
		}
		else {
			src = a;
			dst = tmp;
		}
		//clock_t t = clock();
		for (int i = 0; i < len; i += (width << 1)) {
			int mid = i + width;
			if (mid > len)
				mid = len;

			int right = mid + width;
			if (right > len)
				right = len;

			merge(src, i, mid, right, dst);
		}
		//t = clock() - t;
		//printf("width: %d, time: %f ms\n", width, (float)t * 1000 / CLOCKS_PER_SEC);
		toggle++;
	}

	if (toggle & 1)
		memcpy(a, tmp, len * sizeof(tuple_t));
}

void init_tuples(tuple_t *a, int size) {
	for (int i = 0; i < size; i++) {
		a[i].key = i + 1;
		//a[i].value = i + 1;
	}
}

void shuffle_tuples(tuple_t *a, int size) {
	for (int i = 0; i < size; i++) {
		int j = rand() % size;
		tuple_t tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}
}

void generate_dataset1(tuple_t *a, int size) {
	init_tuples(a, size);
	shuffle_tuples(a, size);
}

void generate_dataset2(tuple_t *a, int size) {
	for (int i = 0; i < size; i++) {
		a[i].key = rand() % size;
		//a[i].value = i + 1;
	}
}

int check_tuples_sorted(tuple_t *a, int size) {
	for (int i = 0; i < size - 1; i++) {
		if (a[i].key > a[i + 1].key)
			return 0; // not sorted
	}

	return 1; // sorted
}

void print_tuples(tuple_t *a, int size) {
	if (size > 16)
		size = 16;
	printf("first %d elements: ", 16);
	for (int i = 0; i < size; i++) {
		printf("%d ", a[i].key);
	}
	printf("\n");
}

int merge_join(tuple_t *tuplesR, tuple_t *tuplesS, int numR, int numS, void *output) {
	int i = 0, j = 0, matches = 0;

	while (i < numR && j < numS) {
		if (tuplesR[i].key < tuplesS[j].key)
			i++;
		else if (tuplesR[i].key > tuplesS[j].key)
			j++;
		else {
			matches++;
			j++;
		}
	}

	return matches;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("usage: %s tuples_size", argv[0]);
		return -1;
	}

	int size = atoi(argv[1]);
	assert(size > 0);

	srand(time(NULL));

	tuple_t *a = malloc(size * sizeof(tuple_t));
	assert(a != NULL);

	printf("tuples size: %d, tuples memory: %f MB\n", size, (float)size * sizeof(tuple_t) / 1024 / 1024);

	generate_dataset1(a, size);
	print_tuples(a, size);

	tuple_t *tmp = malloc(size * sizeof(tuple_t));
	assert(tmp != NULL);
	memset(tmp, 0, size * sizeof(tuple_t));

	printf("begin merge sort\n");

	clock_t t = clock();
	//merge_sort(a, 0, size, tmp);
	merge_sort2(a, size, tmp);
	t = clock() - t;
	printf("time: %f ms\n", (float)t * 1000 / CLOCKS_PER_SEC);

	print_tuples(a, size);
	assert(check_tuples_sorted(a, size));

	return 0;

}
