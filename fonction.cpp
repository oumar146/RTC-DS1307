#include "fonction.h"
#include <Wire.h>

#define ADRESSE_SECONDES 0x00
#define ADRESSE_MINUTES 0x01
#define ADRESSE_HEURES 0x02

void ecritureRtc(int adresse, int donnee)
{
  Wire.beginTransmission(0x68); // secondes
  Wire.write(adresse);
  Wire.write(donnee);
  Wire.endTransmission();
}

int lectureRtc(int adresse)
{
  int donnee = -1;
  int dizaines = 0, unites = 0;
  if (adresse == ADRESSE_SECONDES || ADRESSE_MINUTES)
  {
    Wire.beginTransmission(0x68); // transmettre des données à l'adresse 0x00
    Wire.write(adresse);          // écrire 50
    Wire.endTransmission();
    Wire.requestFrom(0X68, 1); // demande de lecture d'un octet à l'adresse 0x00
    while (Wire.available())
    {
      donnee = Wire.read(); // affichage de la valeur de l'octet
    }
    unites = donnee & 0x0F;
    dizaines = donnee & 0x70;
    dizaines = (dizaines >> 4);
    dizaines = dizaines * 10;
    donnee = dizaines + unites;
    return donnee;
  }
  if (adresse == ADRESSE_HEURES)
  {
    Wire.beginTransmission(0x68); // transmettre des données à l'adresse 0x68
    Wire.write(adresse);
    Wire.endTransmission();
    Wire.requestFrom(0X68, 1); // demande de lecture d'un octet à l'adresse 0x68
    while (Wire.available())
    {
      donnee = Wire.read(); // affichage de la valeur de l'octet
    }
    unites = donnee & 0x0F;
    dizaines = donnee & 0x30;
    dizaines = (dizaines >> 4);
    dizaines = dizaines * 10;
    donnee = dizaines + unites;
    return donnee;
  }
  return donnee;
}

int temporisation(int *heure_debut_temporisation, int *minute_debut_temporisation, int *seconde_debut_temporisation, int heure_actuelle, int minute_actuelle, int seconde_actuelle, int duree_en_secondes)
{
  int tempo_terminee = 0;
  int seconde;
  int minute;
  float duree;
  int minute_suivante = 0;
  if (duree_en_secondes <= 60) // si la duree de la temporisation est inférieure ou égale à 1 minutes
  {
    if (*seconde_debut_temporisation + duree_en_secondes >= 60) // si une minuste va s'écoulée durant la temporisation
    {
      if (*minute_debut_temporisation == 59)
      {
        minute_suivante = 0;
        seconde = (*seconde_debut_temporisation + duree_en_secondes) - 60;     // calculer le nombre de seconde restant après la temporisation
        if (minute_actuelle == minute_suivante && seconde_actuelle >= seconde) // test tempo terminée ?
        {
          tempo_terminee = 1; // temporisation terminee
        }
      }
      else
      {
        minute_suivante = *minute_debut_temporisation + 1;
        seconde = (*seconde_debut_temporisation + duree_en_secondes) - 60;     // calculer le nombre de seconde restant après la temporisation
        if (minute_actuelle >= minute_suivante && seconde_actuelle >= seconde) // test tempo terminée ?
        {
          tempo_terminee = 1; // temporisation terminee
        }
      }
    }
    else if (seconde_actuelle >= (*seconde_debut_temporisation + duree_en_secondes)) // test tempo terminee ?
    {
      tempo_terminee = 1; // temporisation terminee
    }
  }
  else
  {
    duree = duree_en_secondes;
    duree = duree / 60;
    minute = duree;
    seconde = (duree - minute) * 60;
    if (*minute_debut_temporisation + minute > 60) // si une minuste va s'écoulée durant la temporisation
    {
      minute = (*minute_debut_temporisation + minute) - 60;                                                                // calculer le nombre de seconde restant après la temporisation
      if (heure_actuelle + 1 && minute_actuelle >= minute && seconde_actuelle >= (*seconde_debut_temporisation + seconde)) // test tempo terminée ?
      {
        tempo_terminee = 1; // temporisation terminee
      }
    }
    else if (minute_actuelle >= (*minute_debut_temporisation + minute) && seconde_actuelle >= (*seconde_debut_temporisation + seconde))
    {
      tempo_terminee = 1;
    }
  }
  return tempo_terminee;
}

int initalisationHeure()
{
  char saisiHeure[2];
  int heure;
  while (Serial.available() == 0)
    ;                             // atendre la saisie d'un caractère
  saisiHeure[0] = Serial.read();  // stocker le caractère saisie
  Serial.print(saisiHeure[0]);    // afficher le caractère saisie sur le terminal
  int decimal = atoi(saisiHeure); // conversion du caractère saisie en entier
  int unite = atoi(saisiHeure);
  decimal = (decimal << 4); // décalage de 4 bits vers la gauche

  while (Serial.available() == 0)
    ;                            // atendre la saisie d'un caractère
  saisiHeure[0] = Serial.read(); // stocker le caractère saisie
  Serial.print(saisiHeure[0]);   // afficher le caractère saisie sur le terminal

  if (saisiHeure[0] != 0xD)
  {
    unite = atoi(saisiHeure); // conversion du caractère saisie en entier
    heure = decimal + unite;  // initialisation de l'heure saisie dans le champs heures
  }
  else
  {
    decimal = 0;
    heure = unite; // initialisation de l'heure saisie dans le champs heures
  }
  return heure;
}
int initalisationMinute()
{
  char saisiHeure[2];
  int minute = 0;
  while (Serial.available() == 0)
    ;                             // atendre la saisie d'un caractère
  saisiHeure[0] = Serial.read();  // stocker le caractère saisie
  Serial.print(saisiHeure[0]);    // afficher le caractère saisie sur le terminal
  int decimal = atoi(saisiHeure); // conversion du caractère saisie en entier
  decimal = (decimal << 4);       // décalage de 4 bits vers la gauche
  while (Serial.available() == 0)
    ;                            // atendre la saisie d'un caractère
  saisiHeure[0] = Serial.read(); // stocker le caractère saisie
  Serial.print(saisiHeure[0]);   // afficher le caractère saisie sur le terminal
  int unite = atoi(saisiHeure);  // conversion du caractère saisie en entier
  minute = decimal + unite;      // initialisation de l'heure saisie dans le champs heures
  return minute;
}
