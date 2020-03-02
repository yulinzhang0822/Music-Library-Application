#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

#define STRMAX 100
#define SONGMAX 1024

struct Song {
    char title[STRMAX];
    char artist[STRMAX];
    int year_published;
};

struct Song music_library[SONGMAX];
int current_number_of_songs = 0;
const char *fileName = NULL;

//open file and return with FILE*
FILE * open_file(const char *fileName, const char *mode) {
    FILE *fp = fopen(fileName, mode);
    if(fp==NULL) {
        fprintf(stderr, "Can't open %s!\n", fileName);
        exit(1);
    }
    return fp;
}

//swap two songs in the music library
void swap(int i, int j) {
    Song tmpSong_1, tmpSong_2;
    
    strcpy(tmpSong_1.title, music_library[i].title);
    strcpy(tmpSong_1.artist, music_library[i].artist);
    tmpSong_1.year_published = music_library[i].year_published;
    
    strcpy(tmpSong_2.title, music_library[j].title);
    strcpy(tmpSong_2.artist, music_library[j].artist);
    tmpSong_2.year_published = music_library[j].year_published;
    
    strcpy(music_library[i].title, tmpSong_2.title);
    strcpy(music_library[i].artist, tmpSong_2.artist);
    music_library[i].year_published = tmpSong_2.year_published;
    
    strcpy(music_library[j].title, tmpSong_1.title);
    strcpy(music_library[j].artist, tmpSong_1.artist);
    music_library[j].year_published = tmpSong_1.year_published;
}

//convert string to lowercase
void strlwr(char *str)
{
  char *pointer = (char *)str;
  while (*pointer) {
      *pointer = tolower((char)*pointer);
      pointer++;
  }
}

//compare two songs by their titles
int compare_song_titles(char *title_1, char *title_2) {
    char tmp_title_1[STRMAX], tmp_title_2[STRMAX];
    strcpy(tmp_title_1, title_1);
    strcpy(tmp_title_2, title_2);
    strlwr(tmp_title_1);
    strlwr(tmp_title_2);
    return strcmp(tmp_title_1, tmp_title_2);
}

//partition in quicksort
int partition (int low, int high) {
    int pivot = high;
    int i = low - 1;
    for (int j = low; j <= high- 1; j++) {
        if (compare_song_titles(music_library[j].title, music_library[pivot].title) < 0) {
            i++;
            swap(i, j);
        }
    }
    swap(i+1, high);
    return i + 1;
}

//quicksort algorithm
void quick_sort(int low, int high) {
    if (low < high) {
        int pivot = partition(low, high);
        quick_sort(low, pivot - 1);
        quick_sort(pivot + 1, high);
    }
}

//sort music library by title
void sort_MusicLibrary() {
    int low = 0, high = current_number_of_songs-1;
    quick_sort(low, high);
}

//parse a song info
void load_MusicLibrary(char *buf, int current_number_of_songs) {
    char *token = strtok(buf,",");
    if(token!=NULL) {
        strcpy(music_library[current_number_of_songs].title, token);
    }
    token = strtok(NULL,",");
    if(token!=NULL) {
        strcpy(music_library[current_number_of_songs].artist, token);
    }
    token = strtok(NULL,"\n");
    if(token!=NULL) {
        music_library[current_number_of_songs].year_published = atoi(token);
    }
}

//write a song to file
void write_song(FILE *ofp, int index) {
    fprintf(ofp, "%s,", music_library[index].title);
    fprintf(ofp, "%s,", music_library[index].artist);
    fprintf(ofp, "%d\n", music_library[index].year_published);
}

//check if music library has already reaches the maximum capacity
bool check_if_reach_maximum_capacity() {
    if(current_number_of_songs==SONGMAX) {
        printf("The music library reaches maximum capacity!\n");
        return true;
    }
    return false;
}

//read songs from file
void read_song(FILE *ifp) {
    fseek (ifp, 0, SEEK_END);
    int size = int(ftell(ifp));
    if(size!=0) {
        fseek(ifp, 0, SEEK_SET);
        char buf[256];
        while(fgets(buf, sizeof(buf), ifp)) {
            load_MusicLibrary(buf, current_number_of_songs);
            current_number_of_songs++;
            if(check_if_reach_maximum_capacity()) {
                break;
            }
        }
        sort_MusicLibrary();
    } else {
        printf("The file %s has no songs!\n", fileName);
    }
    fclose(ifp);
}

//write songs to file and clear library
void store_MusicLibrary(FILE *ofp) {
    for(int index=0; index<current_number_of_songs; index++) {
        write_song(ofp, index);
    }
    current_number_of_songs = 0;
    fclose(ofp);
}

//print a song info
void print_MusicInfo(int index) {
    printf("%d Title: %s, Artist: %s, Year Published: %d\n", index+1, music_library[index].title, music_library[index].artist, music_library[index].year_published);
}

//print all songs in the music library
void print_MusicLibrary(bool terminal_or_file) {
    if(terminal_or_file) {
        for(int index=0; index<current_number_of_songs; index++) {
            print_MusicInfo(index);
        }
    } else {
        FILE *ofp = open_file(fileName, "w+");
        store_MusicLibrary(ofp);
    }
}

//auxiliary function, called by function remove_song_from_MusicLibrary_by_name()
void crunch_up_from_index(int index) {
    while(index<current_number_of_songs) {
        strcpy(music_library[index].title, music_library[index+1].title);
        strcpy(music_library[index].artist, music_library[index+1].artist);
        music_library[index].year_published = music_library[index+1].year_published;
        index++;
    }
}

//auxiliary function, not used
void crunch_down_from_index(int index) {
    int cur_index = current_number_of_songs-1;
    while(cur_index>index) {
        strcpy(music_library[cur_index].title, music_library[cur_index-1].title);
        strcpy(music_library[cur_index].artist, music_library[cur_index-1].artist);
        music_library[cur_index].year_published = music_library[cur_index-1].year_published;
        cur_index--;
    }
}

//binary search algorithm
int binary_search(int low, int high, char *song_title) {
    char tmp_song_title[STRMAX];
    strcpy(tmp_song_title, song_title);
    while(low<=high) {
        int mid = low + (high - low)/2;
        int num = compare_song_titles(song_title, music_library[mid].title);
        if(num == 0) {
            return mid;
        } else if(num < 0) {
            high = mid - 1;
        } else if(num > 0) {
            low = mid + 1;
        }
    }
    return -1;
}

//find index by calling function binary_search()
int find_index_of_song_with_name(char *song_title) {
    int low = 0, high = current_number_of_songs-1;
    return binary_search(low, high, song_title);
}

//add song to the music library
void add_song_to_MusicLibrary(char *song_title, char *song_artist, int year_published) {
    int index = current_number_of_songs;
    strcpy(music_library[index].title, song_title);
    strcpy(music_library[index].artist, song_artist);
    music_library[index].year_published = year_published;
    current_number_of_songs++;
    sort_MusicLibrary();
}

//check if the input of year is a valid, called by function input_song_info()
bool is_numeric(char *pointer) {
    int length = int(strlen(pointer)), i = 0;
    while (i<length) {
        char ch = (char)pointer[i++];
        if('0'>ch || '9'<ch) {
            return false;
        }
    }
    return true;
}

//input song information, related with command "I"
void input_song_info() {
    char song_title[STRMAX], song_artist[STRMAX], year_published[STRMAX];
    string tmpStr;
    printf("Title: ");
    scanf(" %[^\n]s", song_title);
    printf("Artist: ");
    scanf(" %[^\n]s", song_artist);
    printf("Year Published: ");
    scanf(" %[^\n]s", year_published);
    while(!is_numeric(year_published)) {
        printf("Year Published: ");
        scanf(" %[^\n]s", year_published);
    }
    int year = atoi(year_published);
    add_song_to_MusicLibrary(song_title, song_artist, year);
}

//remove a song from the library by calling function find_index_of_song_with_name()
void remove_song_from_MusicLibrary_by_name() {
    char song_title[STRMAX];
    printf("Title: ");
    scanf(" %[^\n]s", song_title);
    int index = find_index_of_song_with_name(song_title);
    if(index==-1) {
        printf("%s is not found in the music library!\n", song_title);
    } else {
        crunch_up_from_index(index);
        current_number_of_songs--;
        printf("%s has been deleted successfully!\n", song_title);
    }
}

//when look up, there may be some songs with same titles
void check_titles_to_left(char *song_title, int index) {
    int low = 0, cur = index - 1;
    while(cur>=low) {
        if(compare_song_titles(song_title, music_library[cur].title) == 0) {
            printf("%d Title: %s, Artist: %s, Year Published: %d\n", cur+1, music_library[cur].title, music_library[cur].artist, music_library[cur].year_published);
            cur--;
        } else {
            break;
        }
    }
}

//when look up, there may be some songs with same titles
void check_titles_to_right(char *song_title, int index) {
    int high = current_number_of_songs - 1, cur = index + 1;
    while(cur<=high) {
        if(compare_song_titles(song_title, music_library[cur].title) ==0) {
            printf("%d Title: %s, Artist: %s, Year Published: %d\n", cur+1, music_library[cur].title, music_library[cur].artist, music_library[cur].year_published);
            cur++;
        } else {
            break;
        }
    }
}
           
//look up a song in the library by calling function find_index_of_song_with_name()
void look_up_song_in_MusicLibrary() {
    char song_title[STRMAX];
    printf("Title: ");
    scanf(" %[^\n]s", song_title);
    int index = find_index_of_song_with_name(song_title);
    if(index==-1) {
        printf("%s is not found in the music library!\n", song_title);
    } else {
        printf("%d Title: %s, Artist: %s, Year Published: %d\n", index+1, music_library[index].title, music_library[index].artist, music_library[index].year_published);
        check_titles_to_left(song_title, index);
        check_titles_to_right(song_title, index);
    }
}

//evaluate a command
bool evaluate_command(char *command) {
    bool isQuit = false;
    int length = int(strlen(command));
    if(length!=1) {
        return isQuit;
    }
    char com = toupper(command[0]);
    switch(com) {
        case 'I':
            if(check_if_reach_maximum_capacity()) {
                break;
            }
            input_song_info(); break;
        case 'P':
            print_MusicLibrary(true); break;
        case 'D':
            remove_song_from_MusicLibrary_by_name(); break;
        case 'L':
            look_up_song_in_MusicLibrary(); break;
        case 'Q':
            print_MusicLibrary(false);
            isQuit = true;
    }
    return isQuit;
}

//read command from user input
void read_command() {
    printf("myMusic Command: ");
    char command[STRMAX];
    scanf(" %[^\n]s", command);
    while(!evaluate_command(command)) {
        printf("myMusic Command: ");
        scanf(" %[^\n]s", command);
    }
}

//the starting point of the program
int main(int argc, const char *argv[]) {
    fileName = argc==2? argv[1]:"myMusic";
    FILE *ifp = open_file(fileName, "a+");
    read_song(ifp);
    read_command();
    return 0;
}

