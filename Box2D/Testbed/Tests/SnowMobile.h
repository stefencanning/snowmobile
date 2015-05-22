
#ifndef SNOWMOBILE_H
#define SNOWMOBILE_H
#include <math.h>
using namespace std;

class SnowMobile : public Test
{
public:
	SnowMobile()
	{

		maxSpeed=-5;
		scale=5;
		speed=0;
		torque=100.0f;
		TPow = 3;
		SPow=1;
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 0.0f;
			fd.friction = 0.6f;

			shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
			ground->CreateFixture(&fd);

			float32 hs[10] = {0.25f, 1.0f, 4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -2.0f, -1.25f, 0.0f};

			float32 x = 20.0f, y1 = 0.0f, dx = 5.0f;

			for (int32 i = 0; i < 10; ++i)
			{
				float32 y2 = hs[i];
				shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
				ground->CreateFixture(&fd);
				y1 = y2;
				x += dx;
			}

			for (int32 i = 0; i < 10; ++i)
			{
				float32 y2 = hs[i];
				shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
				ground->CreateFixture(&fd);
				y1 = y2;
				x += dx;
			}

			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
			ground->CreateFixture(&fd);

			x += 80.0f;
			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
			ground->CreateFixture(&fd);

			x += 40.0f;
			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 10.0f, 5.0f));
			ground->CreateFixture(&fd);

			x += 20.0f;
			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
			ground->CreateFixture(&fd);

			x += 40.0f;
			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x, 20.0f));
			ground->CreateFixture(&fd);
		}

		// Teeter
		{
			b2BodyDef bd;
			bd.position.Set(140.0f, 1.0f);
			bd.type = b2_dynamicBody;
			b2Body* body = m_world->CreateBody(&bd);

			b2PolygonShape box;
			box.SetAsBox(10.0f, 0.25f);
			body->CreateFixture(&box, 1.0f);

			b2RevoluteJointDef jd;
			jd.Initialize(ground, body, body->GetPosition());
			jd.lowerAngle = -8.0f * b2_pi / 180.0f;
			jd.upperAngle = 8.0f * b2_pi / 180.0f;
			jd.enableLimit = true;
			m_world->CreateJoint(&jd);

			body->ApplyAngularImpulse(100.0f, true);
		}

		// Bridge
		{
			int32 N = 20;
			b2PolygonShape shape;
			shape.SetAsBox(1.0f, 0.125f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			fd.friction = 0.6f;

			b2RevoluteJointDef jd;

			b2Body* prevBody = ground;
			for (int32 i = 0; i < N; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(161.0f + 2.0f * i, -0.125f);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(160.0f + 2.0f * i, -0.125f);
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);

				prevBody = body;
			}

			b2Vec2 anchor(160.0f + 2.0f * N, -0.125f);
			jd.Initialize(prevBody, ground, anchor);
			m_world->CreateJoint(&jd);
		}


		// Car
		{
			b2PolygonShape chassis;
			b2Vec2 vertices[8];
			vertices[0].Set(-3.6f*scale, 0.4f*scale);
			vertices[1].Set(-0.3f*scale, -0.4f*scale);
			vertices[2].Set(1.3f*scale, -0.4f*scale);
			vertices[3].Set(1.9f*scale, 0.0f*scale);
			vertices[4].Set(-1.8f*scale, 0.7f*scale);
			chassis.Set(vertices, 5);

			b2PolygonShape visor;
			b2Vec2 visorVertices[8];
			visorVertices[0].Set(-0.6f*scale, 0.46f*scale);
			visorVertices[1].Set(0.5f*scale, 1.2f*scale);
			visorVertices[2].Set(1.9f*scale, 0.0f*scale);
			visor.Set(visorVertices, 3);

			b2PolygonShape skis;
			b2Vec2 skisVertices[8];
			skisVertices[0].Set(-0.7f*scale, -0.1f*scale);
			skisVertices[1].Set(-0.2f*scale, -0.15f*scale);
			skisVertices[2].Set(0.3f*scale, -0.15f*scale);
			skisVertices[3].Set(0.7f*scale, -0.05f*scale);
			skisVertices[4].Set(0.9f*scale, 0.05f*scale);
			skisVertices[5].Set(0.9f*scale, 0.45f*scale);
			skis.Set(skisVertices, 6);
			



			b2CircleShape circle;
			circle.m_radius = 0.2f*scale;
			float mobileX=15.0f;
			float mobileY=20.0f;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(mobileX, mobileY);
			snowmobile = m_world->CreateBody(&bd);
			snowmobile->CreateFixture(&chassis, 1.0f);

			b2Body* visorBody = m_world->CreateBody(&bd);
			visorBody->CreateFixture(&visor, 1.0f);
			b2WeldJointDef weld;
			weld.bodyA = snowmobile;
			weld.bodyB = visorBody;
			weld.localAnchorA = b2Vec2(0, 0);
			weld.localAnchorB = b2Vec2(0, 0);
			weld.referenceAngle = 0;
			m_world->CreateJoint(&weld);
			
			bd.position.Set(mobileX+(1.3f*scale), mobileY-(0.4f*scale)-(0.35f*scale));
			b2Body* skiBody = m_world->CreateBody(&bd);
			skiBody->CreateFixture(&skis, 1.0f);
			
			b2Vec2 axis(0.0f, 1.0f);
			
			b2WheelJointDef wheelJoint;
			wheelJoint.Initialize(snowmobile, skiBody, skiBody->GetPosition(), axis);
			wheelJoint.motorSpeed = 0.0f;
			wheelJoint.maxMotorTorque = 10.0f;
			wheelJoint.enableMotor = false;
			wheelJoint.frequencyHz = 4.0f;
			wheelJoint.dampingRatio = 0.70f;
			wheelJoint.collideConnected=true;
			b2WheelJoint* skiJoint = (b2WheelJoint*)m_world->CreateJoint(&wheelJoint);





			
			b2PolygonShape teeth;
			teeth.SetAsBox(0.04f*scale, 0.02f*scale);

			b2FixtureDef fd;
			fd.shape = &circle;
			fd.density = 10.0f;
			fd.friction = 0.9f;
			bd.position.Set(mobileX-(3.2f*scale), mobileY-(0.6f*scale));
			gear = m_world->CreateBody(&bd);
			gear->CreateFixture(&fd);

			float centerX = gear->GetPosition().x;
			float centerY = gear->GetPosition().y;
			float pointX = gear->GetPosition().x-(circle.m_radius)-(teeth.m_radius);
			float pointY = gear->GetPosition().y+(circle.m_radius);
			float weldPointX = -(circle.m_radius);
			float weldPointY = 0;
			for(int i =0; i < 12; i++)
			{
				b2FixtureDef fd;
				fd.shape = &teeth;
				fd.density = 200.0f;
				fd.friction = 2.0f;
				float angle = (i*30)* 3.14159265f / 180.0f;
				float x = centerX + (pointX-centerX)*cos(angle) - (pointY-centerY)*sin(angle);
				float y = centerY + (pointX-centerX)*sin(angle) + (pointY-centerY)*cos(angle);
				bd.position.Set(x, y);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);
				b2WeldJointDef weld;
				weld.bodyA = gear;
				weld.bodyB = body;
     
				x = 0 + (weldPointX)*cos(angle) - (weldPointY)*sin(angle);
				y = 0 + (weldPointX)*sin(angle) + (weldPointY)*cos(angle);
				//connect the centers - center in local coordinate - relative to body is 0,0
				weld.localAnchorA = b2Vec2(x, y);
				weld.localAnchorB = b2Vec2((teeth.m_radius), -(teeth.m_radius));
    
				//difference in angle of each body
				weld.referenceAngle = angle;
     
				//add the joint to the world
				m_world->CreateJoint(&weld);
			}

			
			bd.position.Set(mobileX-(1.0f*scale), mobileY-(0.6f*scale));
			b2Body* spinner = m_world->CreateBody(&bd);
			spinner->CreateFixture(&fd);

			centerX = spinner->GetPosition().x;
			centerY = spinner->GetPosition().y;
			pointX = spinner->GetPosition().x-(circle.m_radius)-(teeth.m_radius);
			pointY = spinner->GetPosition().y+(circle.m_radius);
			for(int i =0; i < 12; i++)
			{
				b2FixtureDef fd;
				fd.shape = &teeth;
				fd.density = 200.0f;
				fd.friction = 2.0f;
				float angle = (i*30)* 3.14159265f / 180.0f;
				float x = centerX + (pointX-centerX)*cos(angle) - (pointY-centerY)*sin(angle);
				float y = centerY + (pointX-centerX)*sin(angle) + (pointY-centerY)*cos(angle);
				bd.position.Set(x, y);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);
				b2WeldJointDef weld;
				weld.bodyA = spinner;
				weld.bodyB = body;
     
				x = 0 + (weldPointX)*cos(angle) - (weldPointY)*sin(angle);
				y = 0 + (weldPointX)*sin(angle) + (weldPointY)*cos(angle);
				//connect the centers - center in local coordinate - relative to body is 0,0
				weld.localAnchorA = b2Vec2(x, y);
				weld.localAnchorB = b2Vec2((teeth.m_radius), -(teeth.m_radius));
    
				//difference in angle of each body
				weld.referenceAngle = angle;
     
				//add the joint to the world
				m_world->CreateJoint(&weld);
			}

			
			bd.position.Set(mobileX-(2.1f*scale), mobileY-(0.3f*scale));
			b2Body* holder = m_world->CreateBody(&bd);
			holder->CreateFixture(&fd);



			b2WheelJointDef jd;
			
			jd.Initialize(snowmobile, gear, gear->GetPosition(), axis);
			jd.motorSpeed = 0.0f;
			float powered=1;
			for(int i = 0; i<TPow;i++)
			{
				powered*=scale;
			}
			jd.maxMotorTorque = torque*powered;
			jd.enableMotor = true;
			jd.frequencyHz = 4.0f;
			jd.dampingRatio = 0.7f;
			jd.collideConnected=false;
			gearMotor = (b2WheelJoint*)m_world->CreateJoint(&jd);
			
			jd.Initialize(snowmobile, spinner, spinner->GetPosition(), axis);
			jd.motorSpeed = 0.0f;
			jd.maxMotorTorque = 10.0f;
			jd.enableMotor = false;
			jd.frequencyHz = 4.0f;
			jd.dampingRatio = 0.7f;
			b2WheelJoint* m_spring = (b2WheelJoint*)m_world->CreateJoint(&jd);
			
			jd.Initialize(snowmobile, holder, holder->GetPosition(), axis);
			jd.motorSpeed = 0.0f;
			jd.maxMotorTorque = 10.0f;
			jd.enableMotor = false;
			jd.frequencyHz = 4.0f;
			jd.dampingRatio = 0.7f;
			b2WheelJoint* m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);

			
			{
				float length = 0.0605f*scale;
				b2PolygonShape shape;
				shape.SetAsBox(length, 0.01f*scale);

				b2FixtureDef fd;
				fd.shape = &shape;
				fd.density = 20.0f;
				fd.friction = 0.2f;
				b2FixtureDef td;
				td.shape = &teeth;
				td.density = 20.0f;
				td.friction = 2.0f;

				b2RevoluteJointDef jd;
				jd.localAnchorA.Set(length,0);
				jd.localAnchorB.Set(-length,0);
				jd.collideConnected=true;

				b2DistanceJointDef dd;
				dd.localAnchorA.Set(length,0);
				dd.localAnchorB.Set(-length,0);
				dd.length=0.0f;

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(mobileX-(3.2f*scale)-(circle.m_radius), mobileY-(0.6f*scale));


				b2Body* firstBody = m_world->CreateBody(&bd);
				firstBody->CreateFixture(&fd);

				float centerX = mobileX-(2.1f*scale);
				float centerY = mobileY-(0.6f*scale);
				float pointX = mobileX-(3.2f*scale)-(circle.m_radius)-(teeth.m_radius*4);
				float pointY =  mobileY-(0.6f*scale);

				b2Body* prevBody = firstBody;
				float count = 46;
				for (int32 i = 0; i < count-1; ++i)
				{
					b2BodyDef bd;
					bd.type = b2_dynamicBody;
					float angle = ((i+1)*(360/count))* 3.14159265f / 180.0f;
					float x = centerX + (pointX-centerX)*cos(angle) - (pointY-centerY)*sin(angle);
					float y = centerY + (pointX-centerX)*sin(angle) + (pointY-centerY)*cos(angle);
					bd.position.Set(x, min(max(y,mobileY-(0.6f*scale)-(circle.m_radius)-(teeth.m_radius*4)),mobileY-(0.3f*scale)+(circle.m_radius)+(teeth.m_radius*4)));
					b2Body* body = m_world->CreateBody(&bd);
					body->CreateFixture(&fd);

					if(i%2==0)
					{
						b2Body* body2 = m_world->CreateBody(&bd);
						body2->CreateFixture(&td);

						b2WeldJointDef weld;
						weld.bodyA = body;
						weld.bodyB = body2;
						weld.localAnchorA = b2Vec2(0, 0);
						weld.localAnchorB = b2Vec2(-teeth.m_radius, 0);
						weld.referenceAngle = 90 * 3.14159265f / 180.0f;
						m_world->CreateJoint(&weld);
					}
					jd.bodyA=prevBody;
					jd.bodyB=body;
					m_world->CreateJoint(&jd);

					dd.bodyA=prevBody;
					dd.bodyB=body;
					m_world->CreateJoint(&dd);

					prevBody = body;
				}
			
				jd.bodyA=prevBody;
				jd.bodyB=firstBody;
				m_world->CreateJoint(&jd);
				dd.bodyA=prevBody;
				dd.bodyB=firstBody;
				m_world->CreateJoint(&dd);
			}
		}
	}

	void Keyboard(unsigned char key)
	{
		switch (key)
		{
		case 'a':
			break;

		case 's':
			speed+=0.05;
			gearMotor->SetMotorSpeed(min(speed,0.0f));
			break;

		case 'd':
			speed-=0.05;
			gearMotor->SetMotorSpeed(speed);//max(speed,maxSpeed));
			break;

		case 'q':
			{
				torque-=10;
				float powered=1;
				for(int i = 0; i<TPow;i++)
				{
					powered*=scale;
				}
				gearMotor->SetMaxMotorTorque(torque*powered);
			}
			break;

		case 'e':
			{
				torque+=10;
				float powered=1;
				for(int i = 0; i<TPow;i++)
				{
					powered*=scale;
				}
				gearMotor->SetMaxMotorTorque(torque*powered);
			}
			break;
		}
	}

	void Step(Settings* settings)
	{
		m_debugDraw.DrawString(5, m_textLine, "Keys: brake = s, right = d, torque down = q, torque up = e");
		m_textLine += DRAW_STRING_NEW_LINE;
		m_debugDraw.DrawString(5, m_textLine, "speed = %g, torque = %g",speed,torque);
		m_textLine += DRAW_STRING_NEW_LINE;

		settings->viewCenter.x = snowmobile->GetPosition().x;
		settings->viewCenter.y = snowmobile->GetPosition().y;
		Test::Step(settings);
	}

	static Test* Create()
	{
		return new SnowMobile;
	}

	b2Body* snowmobile;
	b2Body* gear;
	b2WheelJoint* gearMotor;
	float maxSpeed;
	float scale;
	float speed;
	float torque;
	float TPow;
	float SPow;

};

#endif
