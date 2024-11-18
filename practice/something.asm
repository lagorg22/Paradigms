
typedef struct {
    char *name;          0 (4 byte)
    short freeCredits;   4 (2 byte)
    short *grades;       6 (4 byte)
    short (*addScores)(void *scores, int n); 10 (4 byte)_
} Student; 14 bytes

typedef struct {
    char *name;     0 (4 byte)
    short credits;  4 (2 byte)
    short studentsNumber; 6 (2 byte)
    Student **students;   8 (4 byte)
} Course; 12 byte
// short studentScore(Course *course, int studentIndex, int defaultScore) {

<studentScore>:
// int result = 0;
	addi sp, sp, -4
	sw x0, 0(sp)
// short scores[4];
	addi sp, sp, -8
// if ((Student*)(course->students[studentIndex]->name)->addScores((void*)scores, 4) >= 40) {
// course->students[studentIndex]->name
	lw x11, 12(sp) # x11 = course
	lw x12, 8(x11) # x12 = course->students
	lw x13, 16(sp) # x13 = studentIndex
	addi x14, x0, 4 # x14 = 4
	mul x15, x14, x13 # x15 = studentIndex * sizeof(Student *)
	add x16, x12, x15 # x16 = &course->students[studentIndex]
	lw x17, 0(x16) # course->students[studentIndex] = &course->students[studentIndex]->name
	lw x18, 0(x17) # course->students[studentIndex]->name
	lw x19, 10(x18) # x19 = ...->addScores

	addi sp, sp, -12
	sw ra, 8(sp)
	addi x11, x0, 4
	sw x11, 4(sp)
	addi x12, sp, 12
	sw x12, 0(sp)
	jalr x19
	lw ra, 8(sp)
	addi sp, sp, 12
	addi x11, x0, 40
	BLT x10, x11, LESS_THAN_FORTY
// result += scores[0] * course->credits;
	lh x11, 0(sp) # x11 = scores[0]
	lw x12, 12(sp) # x12 = course
	lh x13, 4(x12) # x13 = course->credits
	mul x14, x11, x13 # scores[0] * course->credits
	lw x15, 8(sp) # result
	add x15, x15, x14 # result + scores[0] * course->credits
	sw x15, 8(sp)
LESS_THAN_FORTY:
// } else { result = defaultScore;}
	lw x11, 20(sp) # defaultScore
	sw x11, 8(sp) # result = defaultScore
// return result;
	lw x10, 8(sp)
	addi sp, sp, 12
	ret