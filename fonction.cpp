//il est important d'inclure la bibliothèue Wire.h car le module RTC DS1307 utilise le protocole I2C 
#include <Wire.h>

//adresse des registres dans le module RTC
#define ADRESSE_SECONDES 0x00
#define ADRESSE_MINUTES 0x01
#define ADRESSE_HEURES 0x02

/*fonction permettante d'écrire dans un registre du module RTC :
- La fonction ne retourne rien

- Exemple : 
  paramètre 1 : 0X00 pour écrire dans le registre des secondes
  paramètre 2 : 0x30 pour 30 secondes
*/
void ecritureRtc(int adresse, int donnee)
{
  Wire.beginTransmission(0x68); // secondes
  Wire.write(adresse);
  Wire.write(donnee);
  Wire.endTransmission();
}
/*fonction permettante de lire dans un registre du module RTC :
- La fonction retourne les données en entier

- Exemple : 
  paramètre 1 : 0X01 pour lire dans le registre des minutes
  La fonction retourne 45 (int) 
*/
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

/*fonction permettante de créer une temporisation :
- retourne 1 (int) si la temporisation est terminée
- retourne 0 (int) si la temporisation n'est pas terminée

- Exemple d'une temporisation de 600 secondes (10 min) déclencher à 12h50 : 
  paramètre 1 : 0x12
  paramètre 2 : 0x50
  paramètre 3 : 0x00
  paramètre 4 : 0x12
  paramètre 5 : 0x57
  paramètre 6 : 0x40
  paramètre 7 : 0x600

  Resultat :La fonction retournera 0 car les 600 secondes ne sont pas encore passés

*/

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

