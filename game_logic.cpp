#include "32blit.hpp"

int genRandomPiece(int arr[]){
    
    int pos, val;
    int max_rand = 15;

    // lower the changes of appearing '4'
    for (int i = 0; i < 4; i++){
        val = ((blit::random() % 2) == 0) ? 2 : 4;
        if (val == 2){
            break;
        }
    }
    
    while (max_rand != 0){
        pos = blit::random() % 16;
        if (arr[pos] == 0){
            arr[pos] = val;
            return 0;
        }
        max_rand--;
    }

    //did not find with rand(); try search
    for (int i = 0; i < 16; i++){
        if (arr[i] == 0){
            arr[i] = val;
            return 0;
        }
    }
    
    // we should not reach here. error
    return 1;
}

bool pushZerosInRaw(int arr[], int start, int end, int step){
    bool moved = false;
    //1 - forward; -1 - backward
    int direction = (start - end) < 0 ? 1 : -1;
    int firstNonZero;
    if (direction == 1){
        firstNonZero = start;
        for (int i = start; i < end; i = i + step){
            if (arr[i] != 0){
                if (i != firstNonZero){
                    arr[firstNonZero] = arr[i];
                    arr[i] = 0;
                    moved = true;
                }
                firstNonZero = firstNonZero + step;
            }
        }
    }
    if (direction == -1){
        firstNonZero = start - 1;
        for (int i = start - 1; i >= end; i = i + step){
            if (arr[i] != 0){
                if (i != firstNonZero){
                    arr[firstNonZero] = arr[i];
                    arr[i] = 0;
                    moved = true;
                }
                firstNonZero = firstNonZero + step;
            }
        }
    }
    return moved;
}

bool mergeRaw(int arr[], int start, int end, int step, uint32_t *score){
    bool moved = false;
    //1 - forward; -1 - backward
    int direction = (start - end) < 0 ? 1 : -1;
    if (direction == 1){
        for (int i = start; i < end - step; i = i + step){
            if (arr[i] != 0 && arr[i] == arr[i + step]){
                arr[i] *= 2;
                *score += arr[i + step];
                arr[i + step] = 0;
                moved = true;        
            }
        }
    }
    if (direction == -1){
        for (int i = start - 1; i >= end - step; i = i + step){
            if (arr[i] != 0 && arr[i] == arr[i + step]){
                arr[i] *= 2;
                *score += arr[i + step];
                arr[i + step] = 0;
                moved = true;
            }
        }
    }
    return moved;
}

bool moveLeft(int arr[], uint32_t *score){
    bool moved = false;
    for (int i = 0; i < 4; i++){
        moved |= pushZerosInRaw(arr, i * 4, i * 4 + 4, 1) | mergeRaw(arr, i * 4, i * 4 + 4, 1, score) | 
                 pushZerosInRaw(arr, i * 4, i * 4 + 4, 1);
    }
    return moved;
}

bool moveRight(int arr[], uint32_t *score){
    bool moved = false;
    for (int i = 0; i < 4; i++){
        moved |= pushZerosInRaw(arr, i * 4 + 4, i * 4, -1) | mergeRaw(arr, i * 4 + 4, i * 4, -1, score) | 
                 pushZerosInRaw(arr, i * 4 + 4, i * 4, -1);
    }
    return moved;
}

bool moveUp(int arr[], uint32_t *score){
    bool moved = false;
    for (int i = 0; i < 4; i++){
        moved |= pushZerosInRaw(arr, i, 16, 4) | mergeRaw(arr, i, 16, 4, score) | pushZerosInRaw(arr, i, 16, 4);
    }
    return moved;
}

bool moveDown(int arr[], uint32_t *score){
    bool moved = false;
    for (int i = 0; i < 4; i++){
        moved |= pushZerosInRaw(arr, 16 - i, 0, -4) | mergeRaw(arr, 16 - i, 0, -4, score) | pushZerosInRaw(arr, 16 - i, 0, -4);
    }
    return moved;
}

bool canMove(int arr[]){
    // if there is an empty slot we can move
    for (int i = 0; i < 16; i++){
        if(arr[i] == 0){
            return true;
            break;
        }
    }

    // if numbers can be merged we can move
    // these nested loops only checks 3x3 field
    // x x x o
    // x x x o
    // x x x o
    // o o o 0
    for (int x = 0; x < 3; x++){     
        for (int y = 0; y < 3; y++){
            if (arr[x + 4*y] == arr[x + 4*y +1]){
                 return true;
                 break;
            }
            if (arr[x + 4*y] == arr[x + 4*y + 4]){
                 return true;
                 break;
            }
        }
    }
    
    // now let's check the final raw
    for (int i = 12; i < 15; i++){
        if (arr[i] == arr[i + 1]){
            return true;
            break;
        }
    }

    // and the final column
    for (int i = 3; i < 12; i = i + 4){
        if (arr[i] == arr[i + 4]){
            return true;
            break;
        }
    }


    return false;
}