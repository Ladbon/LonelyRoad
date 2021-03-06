//GameObjectManager.cpp

#include "GameObjectManager.h"
#include "GameObject.h"
#include "DrawManager.h"
#include "SpriteManager.h"
#include "PlayerObject.h"
#include "Truck.h"
#include "EnemyObject.h"
#include "SuperEnemy.h"
#include "Spawner.h"
#include "Button.h"
#include "Config.h"
#include "RepairKit.h"
#include "SniperGirl.h"
#include "Score.h"
#include "EyeCandy.h"
#include "SoundManager.h"

#include <iostream>
#include "HpBar.h"


GameObjectManager::GameObjectManager(SpriteManager* sm, sf::RenderWindow* rw, InputManager* input, SoundManager* soundmngr, std::string* controltype)
{
	m_controltype = controltype;
	m_soundmanager = soundmngr;
	m_input = input;
	m_spritemanager=sm;
	m_window=rw;
	m_truck=nullptr;	//Make sure everything is cleared.
	m_player=nullptr;
	m_spawner=nullptr;
	m_background=nullptr;
	m_enemies.clear();
	m_supers.clear();
	m_girls.clear();
	m_player_projectiles.clear();
	m_enemy_projectiles.clear();
	m_vRepairKits.clear();
	m_hpbar = nullptr;
	m_xscore = nullptr;
	m_eyecandy = nullptr;

}

GameObjectManager::~GameObjectManager()
{
	//	m_spritemanager=nullptr;
	int i = 0;
}

void GameObjectManager::CreateGameObjects(float degree)
{
	//background
	m_background = m_spritemanager->Load("../data/sprites/Background.png", "Wackground", 1, 1);
	m_background->setPosition(0,0);

	//Creates all objects that exists from the beginning

	//ANIMATIONS TRUCK

	m_truck = new Truck(1,m_spritemanager->Load("../data/sprites/TruckLVLOneSpriteSheet.png", "Truck_Animation_lv1",1, 1), 
		m_spritemanager->Load("../data/sprites/weardown2.PNG", "TruckWearDown_1", 1, 1),
		m_spritemanager->Load("../data/sprites/weardown3.PNG", "TruckWearDown_2", 1, 1),
		m_spritemanager->Load("../data/sprites/weardown4.PNG", "TruckWearDown_3", 1, 1));

	//------------------------------------------------------------------





	m_player = new PlayerObject(m_truck, m_input, m_spritemanager->Load("../data/sprites/ArianaSpriteBlack.png", "Ariana", 1, 1), m_spritemanager, m_controltype);
	m_spawner = new Spawner(m_truck);



	//Clears all vectors
	m_enemies.clear();
	m_supers.clear();
	m_girls.clear();
	m_vRepairKits.clear();
	m_player_projectiles.clear();
	m_enemy_projectiles.clear();

	//The game is not over
	m_game_over = false;
	m_hpbar = new HpBar(m_spritemanager->Load("../data/sprites/HP_Bar_2.png", "hpbar", 1,1),
		(m_spritemanager->Load("../data/sprites/HP_Bar_Border_2.png", "hpborder", 1,1)), 
		(m_spritemanager->Load("../data/sprites/HP_Bar_Shadows_2.png", "hpshadow", 1,1)));
	m_xscore = new Score();

	m_eyecandy = new EyeCandy(degree);

	//resets a few things

	m_spawner->m_win = false;
	Kill_Count = 0;
}

void GameObjectManager::ClearGameObjects()
{
	if(m_eyecandy != nullptr){
		delete m_eyecandy;
		m_eyecandy = nullptr;
	};

	if(m_hpbar != nullptr)
	{
		delete m_hpbar;
		m_hpbar = nullptr;
	}
	//Deletes objects and clears vectors. Sprite deletion is fucked up
	if(m_truck != nullptr){
		delete m_truck;
		m_truck = nullptr;
	}
	if(m_player != nullptr){
		delete m_player;
		m_player = nullptr;
	}
	if(m_spawner != nullptr){
		delete m_spawner;
		m_spawner = nullptr;
	}
	//DELETES SCORE
	if(m_xscore !=nullptr)
	{
		delete m_xscore;
		m_xscore = nullptr;
	}
	for (auto it = m_enemies.begin();it != m_enemies.end(); it++)
	{
		if(*it != nullptr) {
			delete *it;
		}
	}
	m_enemies.clear();
	for (auto it = m_supers.begin();it != m_supers.end(); it++)
	{
		if(*it != nullptr) {
			delete *it;
		}
	}
	m_supers.clear();
	for (auto it = m_girls.begin();it != m_girls.end(); it++)
	{
		if(*it != nullptr) {
			delete *it;
		}

	}
	m_girls.clear();
	for (auto it = m_enemy_projectiles.begin();it != m_enemy_projectiles.end(); it++)
	{
		if(*it != nullptr) {
			delete *it;
		}

	}
	m_enemy_projectiles.clear();
	for (auto it = m_player_projectiles.begin();it != m_player_projectiles.end(); it++)
	{
		if(*it != nullptr) {
			delete *it;
		}

	}
	m_player_projectiles.clear();
	for (auto it = m_vRepairKits.begin();it != m_vRepairKits.end(); it++)
	{
		if(*it != nullptr) {
			delete *it;
		}

	}
	m_vRepairKits.clear();
}
//Update
void GameObjectManager::Update(float deltatime)
{
	if(m_spawner->m_win == true && m_enemies.empty())
	{
		m_game_over = true;
	}
	if(m_truck->Update(deltatime)){ //When the truck gets 0 hp it returns true or waves are done
		m_game_over = true;
	};

	m_eyecandy->Update(deltatime);
	if(m_player->Update(deltatime)){ 

		//When the player presses the fire-button Update returns true and a player projectile is push_back'd into the playerbullet vector
		if(m_player->GetWeaponType() == "Revolver") m_soundmanager->PlaySound("M4A1.wav");
		if(m_player->GetWeaponType() == "Needlegun") m_soundmanager->PlaySound("Bow.wav");
		if(m_player->GetWeaponType() == "ArmCannon") m_soundmanager->PlaySound("Bow.wav");
		m_player_projectiles.push_back(new PlayerProjectile
			(m_truck, m_player, m_spritemanager->Load("../data/sprites/BulletProjectile.png", "PlayerBullet", 0.3, 0.3), 
			m_spritemanager->Load("../data/sprites/BulletProjectileNeedle.png", "PlayerNeedle", 1, 1), m_spritemanager->Load("../data/sprites/LAZER.png", "LAZERR", 0.8, 1)));
	}


	//ENEMY WAVES
	m_spawner->UpdateTime(deltatime);

	for(int i = 0; i<m_spawner->NumberOfEnemieslvl1(m_spawner->Wave().x); i++)
	{
		m_enemies.push_back(m_spawner->EnemySpawner(m_spritemanager));
	}

	for(int i = 0; i<m_spawner->NumberOfEnemieslvl2(m_spawner->Wave().y); i++)
	{
		m_supers.push_back(m_spawner->SuperSpawner(m_spritemanager));
	}

	for(int i = 0; i<m_spawner->NumberOfEnemieslvl3(m_spawner->Wave().z); i++)
	{
		m_girls.push_back(m_spawner->SniperSpawner(m_spritemanager));
	}

	m_spawner->NextWaveCheck();
	//END OF ENEMY WAVES

	for(int i = 0; i<m_enemies.size(); i++){ 
		//Updates lvl 1 enemies.
		if(m_enemies.at(i)!=nullptr){
			if(m_enemies.at(i)->Update(deltatime)){ 
				//Update returns true when enemy are close to the truck and their fire-cooldown is 0, 
				//a bullet is pushbacked into the enemybullet vector
				m_soundmanager->PlaySound("M4A1.wav");
				m_enemy_projectiles.push_back(new EnemyProjectile(m_enemies.at(i)->GetDamage(), m_truck, m_enemies.at(i)->GetPosition(),
					m_spritemanager->Load("../data/sprites/BulletProjectile.png", "PlayerBullet", 0.3f, 0.3f)));
			}
		}
	};

	for(int i = 0; i<m_supers.size(); i++){ 
		//Updates lvl 2 enemies.
		if(m_supers.at(i)!=nullptr){
			if(m_supers.at(i)->Update(deltatime)){ 
				//Update returns true when enemy are close to the truck and their fire-cooldown is 0, 
				//a bullet is pushbacked into the enemybullet vector
				m_soundmanager->PlaySound("M4A1.wav");
				m_enemy_projectiles.push_back(new EnemyProjectile(m_supers.at(i)->GetDamage(), m_truck, m_supers.at(i)->GetPosition(),
					m_spritemanager->Load("../data/sprites/BulletProjectile.png", "PlayerBullet", 0.3f, 0.3f)));
			}
		}
	};
	for(int i = 0; i<m_girls.size(); i++){ 
		//Updates lvl 3 enemies.
		if(m_girls.at(i)!=nullptr){
			if(m_girls.at(i)->Update(deltatime)){ 
				//Update returns true when enemy are close to the truck and their fire-cooldown is 0, 
				//a bullet is pushbacked into the enemybullet vector
				m_soundmanager->PlaySound("Sniper.wav");
				m_enemy_projectiles.push_back(new EnemyProjectile(m_girls.at(i)->GetDamage(), m_truck, m_girls.at(i)->GetPosition(),
					m_spritemanager->Load("../data/sprites/BulletProjectile.png", "PlayerBullet", 0.3f, 0.3f)));
			}
		}
	};


	for(int i = 0; i< m_enemy_projectiles.size(); i++){ 
		//Updates all enemy projectiles. The return true if they collide with the truck. The truck is also damaged.
		if(m_enemy_projectiles.at(i)->Update(m_truck, deltatime)){
			delete m_enemy_projectiles[i];
			m_enemy_projectiles.erase(m_enemy_projectiles.begin()+i);
			i--;
		};
	};


	for(int i = 0; i< m_player_projectiles.size(); i++){
		if(m_player_projectiles.at(i)->Update(m_truck, deltatime)){
			delete m_player_projectiles[i];
			m_player_projectiles.erase(m_player_projectiles.begin()+i);
			i--;
		};
	};

	for(int i = 0; i < m_player_projectiles.size(); i++){

		for(int j = 0; j<m_enemies.size(); j++){

			if(m_spawner->EnemyDestroyer(m_enemies.at(j), m_player_projectiles.at(i))){
				if(m_player->GetWeaponType() == "ArmCannon"){
					m_eyecandy->ShockCreator(m_player_projectiles.at(i)->GetPosition());
				}
				else if(m_player->GetWeaponType() == "BoomWosh")
				{
					m_eyecandy->BoomWoshCreator(m_enemies.at(j)->GetPosition(), m_truck->GetPosition());
				}

				else m_eyecandy->BloodCreator("Player", m_player_projectiles.at(i)->GetPosition(), m_player_projectiles.at(i)->GetVelocity());

				if(m_enemies.at(j)->Damaged(m_player->GetDamage())<=0){
					//EYECANDY SCORE AND DEADPICTURE CREATION
					
					m_eyecandy->PictureCreator(m_spritemanager->Load("../data/sprites/DeadBandit.png", "BanditCorpse", 1.2, 1.2), m_enemies.at(j)->GetPosition(), m_player_projectiles.at(i)->GetRotation()+160);
					m_eyecandy->TextCreator(m_xscore->FeedbackScore(10), m_enemies.at(j)->GetPosition());

					int chance = rand()%10;
					if(chance == 0)
					{
						m_vRepairKits.push_back(new RepairKit(m_enemies.at(j)->GetPosition(), m_enemies.at(j)->GetVelocity(), 
							m_spritemanager->Load("../data/sprites/ToolBox.png", "Toolbox", 0.8f, 0.8f)));
					}

					delete m_enemies[j];
					m_enemies.erase(m_enemies.begin()+j);
					//SCORE COUNT
					Kill_Count++;
					m_xscore->PutInScore(enemyscore = 10);
					--j;
				}
				if(m_player_projectiles[i]->GetType2() != "Needle"){
					delete m_player_projectiles[i];
					m_player_projectiles.erase(m_player_projectiles.begin()+i);
					--i;
				}

				break;
			};
		};
	};

	for(int i = 0; i < m_player_projectiles.size(); i++){
		for(int j = 0; j<m_supers.size(); j++){

			if(m_spawner->SuperDestroyer(m_supers.at(j), m_player_projectiles.at(i))){
				if(m_player->GetWeaponType() == "ArmCannon"){
					m_eyecandy->ShockCreator(m_player_projectiles.at(i)->GetPosition());
				}
				else if(m_player->GetWeaponType() == "BoomWosh")
				{
					m_eyecandy->BoomWoshCreator(m_supers.at(j)->GetPosition(), m_truck->GetPosition());
				}
				else m_eyecandy->BloodCreator("Player", m_player_projectiles.at(i)->GetPosition(), m_player_projectiles.at(i)->GetVelocity());
				if(m_supers.at(j)->Damaged(m_player->GetDamage())<=0){
					int chance = rand()%5;
					if(chance == 0)
					{
						m_vRepairKits.push_back(new RepairKit(m_supers.at(j)->GetPosition(), m_supers.at(j)->GetVelocity(), 
							m_spritemanager->Load("../data/sprites/ToolBox.png", "Toolbox", 0.8f, 0.8f)));
					}

					//score and feedback

					m_eyecandy->PictureCreator(m_spritemanager->Load("../data/sprites/Corpse placeholder.png", "Supercorpse", 1.3, 1.3), m_supers.at(j)->GetPosition(), m_player_projectiles.at(i)->GetRotation()+180);
					m_eyecandy->TextCreator(m_xscore->FeedbackScore(20), m_supers.at(j)->GetPosition());


					delete m_supers[j];
					m_supers.erase(m_supers.begin()+j);
					Kill_Count++;
					m_xscore->PutInScore(enemyscore = 20);

					--j;
				}
				if(m_player_projectiles[i]->GetType2() != "Needle"){
					delete m_player_projectiles[i];
					m_player_projectiles.erase(m_player_projectiles.begin()+i);
					--i;
				}
				break;
			};

		};
	};


	for(int i = 0; i < m_player_projectiles.size(); i++){
		for(int j = 0; j<m_girls.size(); j++){

			if(m_spawner->SniperDestroyer(m_girls.at(j), m_player_projectiles.at(i))){
				if(m_player->GetWeaponType() == "ArmCannon"){
					m_eyecandy->ShockCreator(m_player_projectiles.at(i)->GetPosition());
				}
				else if(m_player->GetWeaponType() == "BoomWosh")
				{
					m_eyecandy->BoomWoshCreator(m_girls.at(j)->GetPosition(), m_truck->GetPosition());
				}
				else m_eyecandy->BloodCreator("Player", m_player_projectiles.at(i)->GetPosition(), m_player_projectiles.at(i)->GetVelocity());
				if(m_girls.at(j)->Damaged(m_player->GetDamage())<=0){
					int chance = rand()%2;
					if(chance == 0)
					{
						m_vRepairKits.push_back(new RepairKit(m_girls.at(j)->GetPosition(), m_girls.at(j)->GetVelocity(), 
							m_spritemanager->Load("../data/sprites/ToolBox.png", "Toolbox", 0.8f, 0.8f)));
					}
					m_eyecandy->TextCreator(m_xscore->FeedbackScore(30), m_girls.at(j)->GetPosition());

					//score feedback
					m_eyecandy->PictureCreator(m_spritemanager->Load("../data/sprites/Bandit_2_Corpse.png", "Snipercorpse", 1, 1), m_girls.at(j)->GetPosition(), m_player_projectiles.at(i)->GetRotation()+90);

					delete m_girls[j];
					m_girls.erase(m_girls.begin()+j);

					//SCORE COUNT
					Kill_Count++;
					m_xscore->PutInScore(enemyscore = 30);
					--j;
				}
				if(m_player_projectiles[i]->GetType2() != "Needle"){
					delete m_player_projectiles[i];
					m_player_projectiles.erase(m_player_projectiles.begin()+i);
					--i;
				}
				break;
			};

		};
	};

	for(int i = 0; i< m_vRepairKits.size(); i++){
		if(m_vRepairKits.at(i)->Update(m_player, deltatime)){

			//score feedback and score
			m_eyecandy->TextCreator(m_xscore->FeedbackScore(25), m_vRepairKits.at(i)->GetPosition());
			m_xscore->PutInScore(enemyscore = 25);

			Kill_Count++;


			m_eyecandy->BoomWoshCreator(m_vRepairKits[i]->GetPosition(), m_truck->GetPosition());

			m_truck->Healed();
			delete m_vRepairKits[i];
			m_vRepairKits.erase(m_vRepairKits.begin()+i);
			i--;
		};
	};

	float lol = m_truck->UpdateHP()/m_truck->m_maxhp;
	if(lol>1){
		lol=1;
	}
	m_hpbar->GetSprite()->setScale(lol, 1.0);


}

sf::Vector2f GameObjectManager::GetStartPosition(GameObject *GO)
{
	return GO->GetPosition();
}

void GameObjectManager::DrawGameObjects(float deltatime)
{
	m_window->draw(*m_background);

	m_eyecandy->DrawPictures(deltatime, m_window);

	m_window->draw(*m_truck->GetSprite()); //Draws truck

	m_window->draw(*m_player->GetSprite()); //Draws player

	m_window->draw(m_xscore->DrawScore()); // draws score


	for(int i=0; i<m_vRepairKits.size(); i++){
		if(m_vRepairKits.at(i)!=nullptr){
			m_window->draw(*m_vRepairKits.at(i)->GetSprite()); // draws all RepairKits
		}
	};
	for(int i=0; i<m_enemies.size(); i++){
		if(m_enemies.at(i)!=nullptr){
			m_window->draw(*m_enemies.at(i)->GetSprite()); // draws all enemies
		}
	};
	for(int i=0; i<m_supers.size(); i++){
		if(m_supers.at(i)!=nullptr){
			m_window->draw(*m_supers.at(i)->GetSprite()); // draws all enemies
		}
	};

	for(int i=0; i<m_player_projectiles.size(); i++){
		if(m_player_projectiles.at(i)!=nullptr){
			m_window->draw(*m_player_projectiles.at(i)->GetSprite());	// draws all player projectiles
		}
	};
	for(int i=0; i<m_enemy_projectiles.size(); i++){
		if(m_enemy_projectiles.at(i)!=nullptr){
			m_window->draw(*m_enemy_projectiles.at(i)->GetSprite());	// draws all enemy projetiles
		}
	};
	for(int i=0; i<m_girls.size(); i++){
		if(m_girls.at(i)!=nullptr){
			m_window->draw(*m_girls.at(i)->GetSprite()); // draws all enemies
		}
	};

	m_eyecandy->DrawParticles(deltatime, m_window);
	
	m_window->draw(*m_hpbar->Sprite2()); //draws hpsprite
	m_window->draw(*m_hpbar->GetSprite()); //Draws hpbar
	m_window->draw(*m_hpbar->Sprite3()); //draws hpbars shadow

	if(m_spawner->WaveCheck() < 10)
	{
		m_window->draw(*m_spawner->WaveTitle());
	}

}
//////////////////////////////////////////////////////////////////////////// :)
///////////////////////////////////Buttons//////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void GameObjectManager::CreateButtons()
{
	m_vButtons.push_back(new Button(m_soundmanager, m_input, "StartGame", "Square", m_spritemanager->Load("../data/buttons/Start_Game.png", "StartGame"),

		(Config::getInt("window_w", 0)/2 - 119), Config::getInt("menu_top_padding", 0)));

	m_vButtons.push_back(new Button(m_soundmanager, m_input, "HighScore", "Square", m_spritemanager->Load("../data/buttons/HighScore.png", "QuitGame"), 
		(Config::getInt("window_w", 0)/2 - 119), (Config::getInt("menu_top_padding", 0) + Config::getInt("button_padding", 0))));

	m_vButtons.push_back(new Button(m_soundmanager, m_input, "Options", "Square" ,m_spritemanager->Load("../data/buttons/Options.png", "QuitGame"), 
		(Config::getInt("window_w", 0)/2 - 119), (Config::getInt("menu_top_padding", 0) + (Config::getInt("button_padding", 0)*2))));

	m_vButtons.push_back(new Button(m_soundmanager, m_input, "QuitGame", "Square", m_spritemanager->Load("../data/buttons/Quit_Game.png", "QuitGame"), 
		(Config::getInt("window_w", 0)/2 - 119), (Config::getInt("menu_top_padding", 0) + (Config::getInt("button_padding", 0)*3))));
};

void GameObjectManager::UpdateButtons()
{


};

void GameObjectManager::DrawButtons()
{
	for(int i=0; i<m_vButtons.size(); i++){
		if(m_vButtons.at(i)!=nullptr){
			m_window->draw(*m_vButtons.at(i)->GetSprite()); // draws all buttons
		}
	}
};

void GameObjectManager::CreateCusomizationButtons()
{
	//Top Suit
	//	m_vCustomizeButtons.push_back(new Button(m_input, "ChangeSuitLeft", "Square",  m_spritemanager->Load("../data/buttons/change_left_button.png", "ChangeSuitLeft"), 
	//	Config::getInt("customize_padding_big", 0), Config::getInt("customize_padding_big", 0)));
	m_vCustomizeButtons.push_back(new Button(m_soundmanager, m_input, "UpgradeSuit", "Square", m_spritemanager->Load("../data/buttons/small_upgrade.png", "UpgradeSuit"), 
		Config::getInt("customize_padding_big", 0), Config::getInt("customize_padding_big", 0)));
	//m_vCustomizeButtons.push_back(new Button(m_input, "ChangeSuitRight", "Square", m_spritemanager->Load("../data/buttons/change_right_button.png", "ChangeSuitRight"), 
	//295, Config::getInt("customize_padding_big", 0)));

	//Top Weapon
	m_vCustomizeButtons.push_back(new Button(m_soundmanager, m_input, "ChangeWeaponLeft", "Square", m_spritemanager->Load("../data/buttons/change_left_button.png", "ChangeWeaponLeft"), 
		354, Config::getInt("customize_padding_big", 0)));
	m_vCustomizeButtons.push_back(new Button(m_soundmanager, m_input, "UpgradeWeapon", "Square", m_spritemanager->Load("../data/buttons/upgrade_weapon.png", "UpgradeWeapon"), 
		413, Config::getInt("customize_padding_big", 0)));
	m_vCustomizeButtons.push_back(new Button(m_soundmanager, m_input, "ChangeWeaponRight", "Square", m_spritemanager->Load("../data/buttons/change_right_button.png", "ChangeWeaponRight"), 
		873, Config::getInt("customize_padding_big", 0)));

	//Top Truck
	//m_vCustomizeButtons.push_back(new Button(m_input, "ChangeTruckLeft", "Square", m_spritemanager->Load("../data/buttons/change_left_button.png", "ChangeTruckLeft"), 
	//932, Config::getInt("customize_padding_big", 0)));
	m_vCustomizeButtons.push_back(new Button(m_soundmanager, m_input, "UpgradeTruck", "Square", m_spritemanager->Load("../data/buttons/small_upgrade.png", "UpgradeTruck"), 
		932, Config::getInt("customize_padding_big", 0)));
	//m_vCustomizeButtons.push_back(new Button(m_input, "ChangeTruckRight", "Square", m_spritemanager->Load("../data/buttons/change_right_button.png", "ChangeTruckRight"), 
	//1182, Config::getInt("customize_padding_big", 0)));

	//back
	m_vCustomizeButtons.push_back(new Button(m_soundmanager, m_input, "Back", "Square", m_spritemanager->Load("../data/buttons/back_button.png", "Back"), 
		Config::getInt("customize_padding_big", 0), ((Config::getInt("window_h", 0) - Config::getInt("customize_padding_big",0) - 64))));

	//Trinkets
	/*m_vCustomizeButtons.push_back(new Button(m_input, "Slot1", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "Slot1"),
	1020, 463));
	m_vCustomizeButtons.push_back(new Button(m_input, "Slot2", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "Slot2"),
	1150, 500));
	m_vCustomizeButtons.push_back(new Button(m_input, "Slot3", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "Slot3"),
	1046, 591));*/

};
void GameObjectManager::DrawCustomizationButtons()
{
	for(int i=0; i<m_vCustomizeButtons.size(); i++){
		if(m_vCustomizeButtons.at(i)!=nullptr){
			m_window->draw(*m_vCustomizeButtons.at(i)->GetSprite()); // draws all buttons
		}
	}
}

void GameObjectManager::CreateTrinketButtons()
{
	//ifstates for sprites needed, this will do for testing the layout

	//top row
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot1", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot1"),
		75 + 123/2, 222));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot2", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		75 + (123*1) + (50*2), 222));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot3", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		75 + (123*2) + (50*3), 222));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot4", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		75 + (123*3) + (50*4), 222));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot5", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		75 + (123*4) + (50*5), 222));

	//mid row
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot1", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot1"),
		156 + 123/2, 343));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot2", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		156 + (123*1) + (50*2), 343));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot3", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		156 + (123*2) + (50*3), 343));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot4", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		156 + (123*3) + (50*4), 343));

	//bot row
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot1", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot1"),
		75 + 123/2, 464));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot2", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		75 + (123*1) + (50*2), 464));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot3", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		75 + (123*2) + (50*3), 464));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot4", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		75 + (123*3) + (50*4), 464));
	m_vTrinketButtons.push_back(new Button(m_soundmanager, m_input, "tSlot5", "Circle", m_spritemanager->Load("../data/buttons/trinket_slot.png", "tSlot3"),
		75 + (123*4) + (50*5), 464));

};

void GameObjectManager::DrawTrinketButtons()
{
	for(int i=0; i<m_vTrinketButtons.size(); i++){
		if(m_vTrinketButtons.at(i)!=nullptr){
			m_window->draw(*m_vTrinketButtons.at(i)->GetSprite()); // draws all buttons
		}
	}

};

void GameObjectManager::EraseButtons(){
	for(int i = 0; i< m_vButtons.size(); i++){
		delete m_vButtons[i];
		m_vButtons.erase(m_vButtons.begin()+i);
		i--;
	};
	m_vButtons.clear();
};

void GameObjectManager::EraseCustomizationButtons(){
	for(int i = 0; i < m_vCustomizeButtons.size(); i++){
		delete m_vCustomizeButtons[i];
		m_vCustomizeButtons.erase(m_vCustomizeButtons.begin()+i);
		i--;
	};
	m_vCustomizeButtons.clear();
};

void GameObjectManager::EraseTrinketButtons(){
	for(int i = 0; i < m_vTrinketButtons.size(); i++){
		delete m_vTrinketButtons[i];
		m_vTrinketButtons.erase(m_vTrinketButtons.begin()+i);
		i--;
	};
	m_vTrinketButtons.clear();
};


//SCORE RELATED STUFF

void GameObjectManager::Buy(int value)
{
	m_xscore->BuyEquipment(value);
}

int GameObjectManager::GetScore(int m_value)
{
	m_value = m_xscore->GetScore();
	return m_value;
}


void GameObjectManager::Dead()
{
	m_window->draw(m_xscore->DrawWhenDead());
}
void GameObjectManager::Won()
{
	m_window->draw(m_xscore->DrawWhenWon());
}

//truck related stuff

void GameObjectManager::UpgradeMaxHpAndSprites()
{
	if(m_truck != nullptr){
		delete m_truck;
		m_truck = nullptr;
	}

	if(Config::getInt("current_truck", 0) == 2)
	{
		m_truck = new Truck(3, m_spritemanager->Load("../data/sprites/truck_lvl3.PNG", "Truck_Animation_lv3",1, 1), 
			m_spritemanager->Load("../data/sprites/weardown_lvl3_2.png", "TruckWearDown_1", 1, 1),
			m_spritemanager->Load("../data/sprites/weardown_lvl3_3.png", "TruckWearDown_2", 1, 1),
			m_spritemanager->Load("../data/sprites/weardown_lvl3_4.png", "TruckWearDown_3", 1, 1));
	}
	if(Config::getInt("current_truck", 0) == 1)
	{
		m_truck = new Truck(2, m_spritemanager->Load("../data/sprites/TruckLvl2SpriteSheet.png", "Truck_Animation_lv2",1, 1), 
			m_spritemanager->Load("../data/sprites/weardown_lvl2_2.png", "TruckWearDown_1", 1, 1),
			m_spritemanager->Load("../data/sprites/weardown_lvl2_3.png", "TruckWearDown_2", 1, 1),
			m_spritemanager->Load("../data/sprites/weardown_lvl2_4.png", "TruckWearDown_3", 1, 1));
	}

}
int GameObjectManager::Kill_count()
{
	return Kill_Count;
}
bool GameObjectManager::GetWin()
{
	return m_spawner->m_win;
}
