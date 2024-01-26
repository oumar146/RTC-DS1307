#ifndef FONCTION_H
#define FONCTION_H

//char receptionMessage(int i, int tailleMessage);
void ecritureRtc(int adresse, int valeur);
int lectureRtc(int adresse);
int initalisationHeure();
int initalisationHeure();
int initalisationMinute();
int temporisation(int *heure_debut_temporisation, int *minute_debut_temporisation, int *seconde_debut_temporisation, int heure_actuelle, int minute_actuelle, int seconde_actuelle, int duree_en_secondes);





#endif