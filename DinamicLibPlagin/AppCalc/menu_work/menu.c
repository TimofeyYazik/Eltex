
int Menu(int count_argument, char **arguments){
  int menu = 0;
  for(int i = 0; i < count_argument; i++){
    printf("%d) %s", i + 1,arguments[i]);
  }
  printf("%d) Exit" , count_argument + 1);
  scanf("%d", &menu);
  return menu;
}



