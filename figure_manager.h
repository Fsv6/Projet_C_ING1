#ifndef FIGURE_MANAGER_H
#define FIGURE_MANAGER_H

typedef struct Figure {
    int id;                  // Identifiant unique
    char type[16];           // Type de la figure ("circle", "square", etc.)
    int x, y;                // Position
    int size;                // Taille (rayon ou côté)
    char color[16];          // Couleur
    int cursor_id;  // stocke l'ID du curseur qui a créé (ou qui "contrôle") cette figure
    struct Figure *next;     // Pointeur vers la figure suivante
} Figure;

// Fonctions pour gérer les figures
Figure *create_figure(const char *type, int x, int y, int size, const char *color, int cursor_id);
Figure *get_figure_by_id(int id);
void delete_figure(int id);
Figure *get_all_figures(void);

#endif // FIGURE_MANAGER_H
