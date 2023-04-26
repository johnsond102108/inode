#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>

int count = 0;

void listFiles(char *path, FILE *fptr){
  //lstat struct
  struct stat a;

  //next path string
  char buffer[1000];

  //dir struct
  struct dirent *d;

  DIR *dir = opendir(path);
  if(!dir){
    return;
  }
  
  while((d = readdir(dir)) != NULL){
    if(strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0){
      //setup for lstat
      char temp[1000];
      strcpy(temp, path);
      strcat(temp, "/");
      strcat(temp, d->d_name);
      lstat(temp, &a);

      //starting directory information
      if(count == 0){
      count = 1;
      	fprintf(fptr,"\t%s[label=\"%s, %ld, %ld, %ld, %ld\"];\n", path, path, a.st_size, a.st_blocks, a.st_ino, a.st_nlink);
      }

      //removing extension so dot is happy
      char augh[1000];
      strcpy(augh, d->d_name);
      char *name = strtok(augh, ".");

      //shortens subdirectory paths to make coherent nodes
      if(strchr(path, '/') != NULL){
	char *ptr = strtok(path, "/");
	ptr = strtok(NULL, "/");
	fprintf(fptr,"\t%s->%s[label=\"%s, %ld, %ld, %ld, %ld\"];\n", ptr, name, d->d_name, a.st_size, a.st_blocks, a.st_ino, a.st_nlink);
      }else{
	fprintf(fptr, "\t%s->%s[label=\"%s, %ld, %ld, %ld, %ld\"];\n", path, name, d->d_name, a.st_size, a.st_blocks, a.st_ino, a.st_nlink);
      }

      //next path
      strcpy(buffer, path);
      strcat(buffer, "/");
      strcat(buffer, d->d_name);
      
      listFiles(buffer, fptr);
    }
  }
  closedir(dir);
}

int main(int argc, char *argv[]){
  FILE *fptr;
  fptr = fopen("graph.gv", "w");
  fprintf(fptr, "digraph sample_flow{\n");
  fprintf(fptr, "\trankdir=LR\n");
  fprintf(fptr, "\tnode [shape=circle];\n");
  char path[100];
  scanf("%s", path);
  listFiles(path, fptr);
  fprintf(fptr, "}");
  fclose(fptr);
}
