/*
Copyright (C) 2005-2007 The Pentagram team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef AUDIOPROCESS_H_INCLUDED
#define AUDIOPROCESS_H_INCLUDED

#include "Process.h"
#include "intrinsics.h"
#include <list>
#include <string>

namespace Pentagram {
	class AudioSample;
}

class AudioProcess :
	public Process
{
	struct SampleInfo {
		int32_t		sfxnum;
		int32_t		priority;
		ObjId		objid;
		int32_t		loops;
		int32_t		channel;
		std::string barked;
		uint32_t		curspeech_start, curspeech_end;
		uint32_t		pitch_shift;	// 0x10000 is normal
		uint16_t		volume;			// 0-256
		int16_t		lvol;
		int16_t		rvol;
		
		SampleInfo() : sfxnum(-1) { }
		SampleInfo(int32_t s,int32_t p,ObjId o,int32_t l,int32_t c,uint32_t ps,uint16_t v, int16_t lv, int16_t rv) : 
			sfxnum(s),priority(p),objid(o),loops(l),channel(c),
			pitch_shift(ps), volume(v), lvol(lv), rvol(rv) { }
		SampleInfo(std::string &b,int32_t shpnum,ObjId o,int32_t c,
				   uint32_t s,uint32_t e,uint32_t ps,uint16_t v, int16_t lv, int16_t rv) : 
			sfxnum(-1),priority(shpnum),objid(o),loops(0),channel(c),barked(b),
			curspeech_start(s), curspeech_end(e), pitch_shift(ps), volume(v), 
			lvol(lv), rvol(rv) { }
	};

	std::list<SampleInfo>	sample_info;

public:
	// p_dynamic_class stuff
	ENABLE_RUNTIME_CLASSTYPE();

	AudioProcess(void);
	virtual ~AudioProcess(void);

	//! Get the current instance of the Audio Processes
	static AudioProcess	* get_instance() { return the_audio_process; }

	INTRINSIC(I_playSFX);
	INTRINSIC(I_playAmbientSFX);
	INTRINSIC(I_isSFXPlaying);
	INTRINSIC(I_setVolumeSFX);
	INTRINSIC(I_stopSFX);

	static void ConCmd_listSFX(const Console::ArgvType &argv);
	static void ConCmd_stopSFX(const Console::ArgvType &argv);
	static void ConCmd_playSFX(const Console::ArgvType &argv);


	virtual void run();

	void playSFX(int sfxnum, int priority, ObjId objid, int loops,
				 bool no_duplicates, uint32_t pitch_shift,
				 uint16_t volume, int16_t lvol, int16_t rvol);

	void playSFX(int sfxnum, int priority, ObjId objid, int loops,
				 bool no_duplicates=false, uint32_t pitch_shift=0x10000,
				 uint16_t volume=0x80) {
		 playSFX(sfxnum, priority, objid, loops, no_duplicates, pitch_shift, volume, -1, -1);
	}

	void stopSFX(int sfxnum, ObjId objid);
	bool isSFXPlaying(int sfxnum);
	void setVolumeSFX(int sfxnum, uint8_t volume);

	bool playSpeech(std::string &barked, int shapenum, ObjId objid, 
					uint32_t pitch_shift=0x10000,uint16_t volume=256);
	void stopSpeech(std::string &barked, int shapenum, ObjId objid);
	bool isSpeechPlaying(std::string &barked, int shapenum);

	//! get length (in milliseconds) of speech
	uint32_t getSpeechLength(std::string &barked, int shapenum) const;

	//! play a sample (without storing a SampleInfo)
	//! returns channel sample is played on, or -1
	int playSample(Pentagram::AudioSample* sample, int priority, int loops, 
				   uint32_t pitch_shift=0x10000, int16_t lvol=256, int16_t rvol=256);

	//! pause all currently playing samples
	void pauseAllSamples();
	//! unpause all currently playing samples
	void unpauseAllSamples();

	//! stop all samples except speech
	void stopAllExceptSpeech();

	// AudioProcess::playSound console command
	//static void ConCmd_playSound(const Console::ArgvType &argv);

	bool loadData(IDataSource* ids, uint32_t version);

private:
	virtual void saveData(ODataSource* ods);
	uint32_t paused;

	//! play the next speech sample for the text in this SampleInfo
	//! note: si is reused if successful
	//! returns true if there was speech left to play, or false if finished
	bool continueSpeech(SampleInfo& si);

	bool calculateSoundVolume(ObjId objid, int16_t &lvol, int16_t &rvol) const;

	static AudioProcess	*	the_audio_process;
};

#endif
