/********************************************************************************
 * Copyright (C) 2024 Han Jinpeng <hanjinpeng127@gmail.com>                     *
 * Website: https://github.com/prownd/systemd-manage                            *
 *                                                                              *
 * This file is part of the systemd-manage project.                             *
 * License: GPLv3                                                               *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the Free   *
 * Software Foundation, either version 3 of the License, or (at your option)    *
 * any later version.                                                           *
 *                                                                              *
 * This program is distributed in the hope that it will be useful, but WITHOUT  *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for     *
 * more details.                                                                *
 *                                                                              *
 * You should have received a copy of the GNU General Public License along      *
 * with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.  *
 ********************************************************************************/

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bchar\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bdouble\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
        QStringLiteral("\\bfriend\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"),
        QStringLiteral("\\blong\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
        QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
        QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsignals\\b"), QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstruct\\b"),
        QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvirtual\\b"),
        QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bbool\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    systemdServiceKeywordFormat.setForeground(Qt::darkCyan);
    //systemdServicekeywordFormat.setFontWeight(QFont::Bold);
    //systemdServicekeywordFormat.setFontPointSize(16);
    const QString systemdServiceKeywordPatterns[] = {
        QStringLiteral("\\bAccept\\b"), QStringLiteral("\\bAccuracySec\\b"), QStringLiteral("\\bAfter\\b"),
        QStringLiteral("\\bAlias\\b"), QStringLiteral("\\bAllowIsolate\\b"), QStringLiteral("\\bAlso\\b"),
        QStringLiteral("\\bAmbientCapabilities\\b"), QStringLiteral("\\bAssertPathExists\\b"), QStringLiteral("\\bAssertPathIsReadWrite\\b"),
        QStringLiteral("\\bBacklog\\b"), QStringLiteral("\\bBefore\\b"), QStringLiteral("\\bBindIPv6Only\\b"),
        QStringLiteral("\\bBindsTo\\b"), QStringLiteral("\\bBusName\\b"), QStringLiteral("\\bCacheDirectory\\b"),
        QStringLiteral("\\bCapabilityBoundingSet\\b"), QStringLiteral("\\bCollectMode\\b"), QStringLiteral("\\bConditionACPower\\b"),
        QStringLiteral("\\bConditionCapability\\b"), QStringLiteral("\\bConditionControlGroupController\\b"), QStringLiteral("\\bConditionCredential\\b"),
        QStringLiteral("\\bConditionDirectoryNotEmpty\\b"), QStringLiteral("\\bConditionEnvironment\\b"), QStringLiteral("\\bConditionFileIsExecutable\\b"),
        QStringLiteral("\\bConditionFileNotEmpty\\b"), QStringLiteral("\\bConditionFirstBoot\\b"), QStringLiteral("\\bConditionKernelCommandLine\\b"),
        QStringLiteral("\\bConditionNeedsUpdate\\b"), QStringLiteral("\\b ConditionPathExists\\b"), QStringLiteral("\\bConditionPathExists\\b"),
        QStringLiteral("\\bConditionPathExistsGlob\\b"), QStringLiteral("\\bConditionPathIsDirectory\\b"), QStringLiteral("\\bConditionPathIsMountPoint\\b"),
        QStringLiteral("\\bConditionPathIsReadWrite\\b"), QStringLiteral("\\bConditionPathIsSymbolicLink\\b"), QStringLiteral("\\bConditionSecurity\\b"),
        QStringLiteral("\\bConditionUser\\b"), QStringLiteral("\\bConditionVirtualization\\b"), QStringLiteral("\\bConfigurationDirectory\\b"),
        QStringLiteral("\\bConfigurationDirectoryMode\\b"), QStringLiteral("\\bConflicts\\b"), QStringLiteral("\\bCoredumpReceive\\b"),
        QStringLiteral("\\bCPUSchedulingPolicy\\b"), QStringLiteral("\\bCPUSchedulingPriority\\b"), QStringLiteral("\\bDefaultDependencies\\b"),
        QStringLiteral("\\bDefaultInstance\\b"), QStringLiteral("\\bDeferAcceptSec\\b"), QStringLiteral("\\bDelegate\\b"),
        QStringLiteral("\\bDelegateSubgroup\\b"), QStringLiteral("\\bDescription\\b"), QStringLiteral("\\bDeviceAllow\\b"),
        QStringLiteral("\\bDevicePolicy\\b"), QStringLiteral("\\bDirectoryMode\\b"), QStringLiteral("\\bDocumentation\\b"),
        QStringLiteral("\\bDynamicUser\\b"), QStringLiteral("\\bEnvironment\\b"), QStringLiteral("\\bEnvironmentFile\\b"),
        QStringLiteral("\\bExecCondition\\b"), QStringLiteral("\\bExecReload\\b"), QStringLiteral("\\bExecStart\\b"),
        QStringLiteral("\\bExecStartPost\\b"), QStringLiteral("\\bExecStartPre\\b"), QStringLiteral("\\bExecStop\\b"),
        QStringLiteral("\\bExecStopPost\\b"), QStringLiteral("\\bFailureAction\\b"), QStringLiteral("\\bFileDescriptorName\\b"),
        QStringLiteral("\\bFileDescriptorStoreMax\\b"), QStringLiteral("\\bGroup\\b"), QStringLiteral("\\bGuessMainPID\\b"),
        QStringLiteral("\\bIgnoreOnIsolate\\b"), QStringLiteral("\\bIgnoreSIGPIPE\\b"), QStringLiteral("\\bImportCredential\\b"),
        QStringLiteral("\\bIOSchedulingClass\\b"), QStringLiteral("\\bIOSchedulingPriority\\b"), QStringLiteral("\\bIOWeight\\b"),
        QStringLiteral("\\bIPAddressAllow\\b"), QStringLiteral("\\bIPAddressDeny\\b"), QStringLiteral("\\bJobTimeoutAction\\b"),
        QStringLiteral("\\bJobTimeoutSec\\b"), QStringLiteral("\\bKeyringMode\\b"), QStringLiteral("\\bKillMode\\b"),
        QStringLiteral("\\bKillSignal\\b"), QStringLiteral("\\bLimitCORE\\b"), QStringLiteral("\\bLimitMEMLOCK\\b"),
        QStringLiteral("\\bLimitNOFILE\\b"), QStringLiteral("\\bLimitNPROC\\b"), QStringLiteral("\\bLimitSTACK\\b"),
        QStringLiteral("\\bListenDatagram\\b"), QStringLiteral("\\bListenFIFO\\b"), QStringLiteral("\\bListenNetlink\\b"),
        QStringLiteral("\\bListenSequentialPacket\\b"), QStringLiteral("\\bListenSpecial\\b"), QStringLiteral("\\bListenStream\\b"),
        QStringLiteral("\\bLockPersonality\\b"), QStringLiteral("\\bLogsDirectory\\b"), QStringLiteral("\\bLogsDirectoryMode\\b"),
        QStringLiteral("\\bMaxConnections\\b"), QStringLiteral("\\bMemoryDenyWriteExecute\\b"), QStringLiteral("\\bMemoryLow\\b"),
        QStringLiteral("\\bMemoryMax\\b"), QStringLiteral("\\bMemoryMin\\b"), QStringLiteral("\\bMemoryPressureWatch\\b"),
        QStringLiteral("\\bMountFlags\\b"), QStringLiteral("\\bNice\\b"), QStringLiteral("\\bNoDelay\\b"),
        QStringLiteral("\\bNonBlocking\\b"), QStringLiteral("\\bNoNewPrivileges\\b"), QStringLiteral("\\bOnActiveSec\\b"),
        QStringLiteral("\\bOnBootSec\\b"), QStringLiteral("\\bOnCalendar\\b"), QStringLiteral("\\bOnFailure\\b"),
        QStringLiteral("\\bOnFailureJobMode\\b"), QStringLiteral("\\bOnStartupSec\\b"), QStringLiteral("\\bOnUnitActiveSec\\b"),
        QStringLiteral("\\bOnUnitInactiveSec\\b"), QStringLiteral("\\bOOMPolicy\\b"), QStringLiteral("\\bOOMScoreAdjust\\b"),
        QStringLiteral("\\bOptions\\b"), QStringLiteral("\\bPAMName\\b"), QStringLiteral("\\bPartOf\\b"),
        QStringLiteral("\\bPassCredentials\\b"), QStringLiteral("\\bPassEnvironment\\b"), QStringLiteral("\\bPassPacketInfo\\b"),
        QStringLiteral("\\bPassSecurity\\b"), QStringLiteral("\\bPermissionsStartOnly\\b"), QStringLiteral("\\bPersistent\\b"),
        QStringLiteral("\\bPIDFile\\b"), QStringLiteral("\\bPriority\\b"), QStringLiteral("\\bPrivateDevices\\b"),
        QStringLiteral("\\bPrivateMounts\\b"), QStringLiteral("\\bPrivateNetwork\\b"), QStringLiteral("\\bPrivateTmp\\b"),
        QStringLiteral("\\bPrivateUsers\\b"), QStringLiteral("\\bProcSubset\\b"), QStringLiteral("\\bProtectClock\\b"),
        QStringLiteral("\\bProtectControlGroups\\b"), QStringLiteral("\\bProtectHome\\b"), QStringLiteral("\\bProtectHostname\\b"),
        QStringLiteral("\\bProtectKernelLogs\\b"), QStringLiteral("\\bProtectKernelModules\\b"), QStringLiteral("\\bProtectKernelTunables\\b"),
        QStringLiteral("\\bProtectProc\\b"), QStringLiteral("\\bProtectSystem\\b"), QStringLiteral("\\bRandomizedDelaySec\\b"),
        QStringLiteral("\\bReadOnlyPaths\\b"), QStringLiteral("\\bReadWritePaths\\b"), QStringLiteral("\\bReceiveBuffer\\b"),
        QStringLiteral("\\bRefuseManualStart\\b"), QStringLiteral("\\bRefuseManualStop\\b"), QStringLiteral("\\bReloadPropagatedFrom\\b"),
        QStringLiteral("\\bRemainAfterExit\\b"), QStringLiteral("\\bRemoveIPC\\b"), QStringLiteral("\\bRemoveOnStop\\b"),
        QStringLiteral("\\bRequiredBy\\b"), QStringLiteral("\\bRequires\\b"), QStringLiteral("\\bRequiresMountsFor\\b"),
        QStringLiteral("\\bRequisite\\b"), QStringLiteral("\\bRestart\\b"), QStringLiteral("\\bRestartForceExitStatus\\b"),
        QStringLiteral("\\bRestartKillSignal\\b"), QStringLiteral("\\bRestartPreventExitStatus\\b"), QStringLiteral("\\bRestartSec\\b"),
        QStringLiteral("\\bRestrictAddressFamilies\\b"), QStringLiteral("\\bRestrictNamespaces\\b"), QStringLiteral("\\bRestrictRealtime\\b"),
        QStringLiteral("\\bRestrictSUIDSGID\\b"), QStringLiteral("\\bRuntimeDirectory\\b"), QStringLiteral("\\bRuntimeDirectoryMode\\b"),
        QStringLiteral("\\bRuntimeDirectoryPreserve\\b"), QStringLiteral("\\bRuntimeMaxSec\\b"), QStringLiteral("\\bSELinuxContext\\b"),
        QStringLiteral("\\bSendBuffer\\b"), QStringLiteral("\\bSendSIGHUP\\b"), QStringLiteral("\\bSendSIGKILL\\b"),
        QStringLiteral("\\bService\\b"), QStringLiteral("\\bSlice\\b"), QStringLiteral("\\bSocketGroup\\b"),
        QStringLiteral("\\bSocketMode\\b"), QStringLiteral("\\bSockets\\b"), QStringLiteral("\\bSocketUser\\b"),
        QStringLiteral("\\bStandardError\\b"), QStringLiteral("\\bStandardInput\\b"), QStringLiteral("\\bStandardOutput\\b"),
        QStringLiteral("\\bStartLimitBurst\\b"), QStringLiteral("\\bStartLimitInterval\\b"), QStringLiteral("\\bStartLimitIntervalSec\\b"),
        QStringLiteral("\\bStateDirectory\\b"), QStringLiteral("\\bStateDirectoryMode\\b"), QStringLiteral("\\bStopWhenUnneeded\\b"),
        QStringLiteral("\\bSuccessAction\\b"), QStringLiteral("\\bSuccessExitStatus\\b"), QStringLiteral("\\bSupplementaryGroups\\b"),
        QStringLiteral("\\bSymlinks\\b"), QStringLiteral("\\bSyslogLevel\\b"), QStringLiteral("\\bSystemCallArchitectures\\b"),
        QStringLiteral("\\bSystemCallErrorNumber\\b"), QStringLiteral("\\bSystemCallFilter\\b"), QStringLiteral("\\bTasksMax\\b"),
        QStringLiteral("\\bTimeoutSec\\b"), QStringLiteral("\\bTimeoutStartSec\\b"), QStringLiteral("\\bTimeoutStopSec\\b"),
        QStringLiteral("\\bTimestamping\\b"), QStringLiteral("\\bTTYPath\\b"), QStringLiteral("\\bTTYReset\\b"),
        QStringLiteral("\\bTTYVHangup\\b"), QStringLiteral("\\bTTYVTDisallocate\\b"), QStringLiteral("\\bType\\b"),
        QStringLiteral("\\bUMask\\b"), QStringLiteral("\\bUnit\\b"), QStringLiteral("\\bUnsetEnvironment\\b"),
        QStringLiteral("\\bUser\\b"), QStringLiteral("\\bUtmpIdentifier\\b"), QStringLiteral("\\bWantedBy\\b"),
        QStringLiteral("\\bWants\\b"), QStringLiteral("\\bWatchdogSec\\b"), QStringLiteral("\\bWhat\\b"),
        QStringLiteral("\\bWhere\\b"), QStringLiteral("\\bWorkingDirectory\\b"), QStringLiteral("\\bWritable\\b"),
    };
    for (const QString &pattern : systemdServiceKeywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = systemdServiceKeywordFormat;
        highlightingRules.append(rule);
    }

    systemdConfigKeywordFormat.setForeground(Qt::darkCyan);
    //systemdConfigKeywordFormat.setFontWeight(QFont::Bold);
    //systemdConfigKeywordFormat.setFontPointSize(16);
    const QString systemdConfigKeywordPatterns[] = {
        QStringLiteral("\\bAllowHibernation\\b"), QStringLiteral("\\bAllowHybridSleep\\b"), QStringLiteral("\\bAllowSuspend\\b"),
        QStringLiteral("\\bAllowSuspendThenHibernate\\b"), QStringLiteral("\\bAudit\\b"), QStringLiteral("\\bCache\\b"),
        QStringLiteral("\\bCacheFromLocalhost\\b"), QStringLiteral("\\bCompress\\b"), QStringLiteral("\\bConnectionRetrySec\\b"),
        QStringLiteral("\\bCPUAffinity\\b"), QStringLiteral("\\bCPUWeight\\b"), QStringLiteral("\\bCrashChangeVT\\b"),
        QStringLiteral("\\bCrashReboot\\b"), QStringLiteral("\\bCrashShell\\b"), QStringLiteral("\\bCtrlAltDelBurstAction\\b"),
        QStringLiteral("\\bDefaultBlockIOAccounting\\b"), QStringLiteral("\\bDefaultCPUAccounting\\b"), QStringLiteral("\\bDefaultDeviceTimeoutSec\\b"),
        QStringLiteral("\\bDefaultEnvironment\\b"), QStringLiteral("\\bDefaultFileSystemType\\b"), QStringLiteral("\\bDefaultIOAccounting\\b"),
        QStringLiteral("\\bDefaultIPAccounting\\b"), QStringLiteral("\\bDefaultLimitAS\\b"), QStringLiteral("\\bDefaultLimitCORE\\b"),
        QStringLiteral("\\bDefaultLimitCPU\\b"), QStringLiteral("\\bDefaultLimitDATA\\b"), QStringLiteral("\\bDefaultLimitFSIZE\\b"),
        QStringLiteral("\\bDefaultLimitLOCKS\\b"), QStringLiteral("\\bDefaultLimitMEMLOCK\\b"), QStringLiteral("\\bDefaultLimitMSGQUEUE\\b"),
        QStringLiteral("\\bDefaultLimitNICE\\b"), QStringLiteral("\\bDefaultLimitNOFILE\\b"), QStringLiteral("\\bDefaultLimitNPROC\\b"),
        QStringLiteral("\\bDefaultLimitRSS\\b"), QStringLiteral("\\bDefaultLimitRTPRIO\\b"), QStringLiteral("\\bDefaultLimitRTTIME\\b"),
        QStringLiteral("\\bDefaultLimitSIGPENDING\\b"), QStringLiteral("\\bDefaultLimitSTACK\\b"), QStringLiteral("\\bDefaultMemoryAccounting\\b"),
        QStringLiteral("\\bDefaultMemoryPressureDurationSec\\b"), QStringLiteral("\\bDefaultMemoryPressureLimit\\b"), QStringLiteral("\\bDefaultMemoryPressureThresholdSec\\b"),
        QStringLiteral("\\bDefaultMemoryPressureWatch\\b"), QStringLiteral("\\bDefaultOOMPolicy\\b"), QStringLiteral("\\bDefaultRestartSec\\b"),
        QStringLiteral("\\bDefaultSmackProcessLabel\\b"), QStringLiteral("\\bDefaultStandardError\\b"), QStringLiteral("\\bDefaultStandardOutput\\b"),
        QStringLiteral("\\bDefaultStartLimitBurst\\b"), QStringLiteral("\\bDefaultStartLimitIntervalSec\\b"), QStringLiteral("\\bDefaultStorage\\b"),
        QStringLiteral("\\bDefaultTasksAccounting\\b"), QStringLiteral("\\bDefaultTasksMax\\b"), QStringLiteral("\\bDefaultTimeoutAbortSec\\b"),
        QStringLiteral("\\bDefaultTimeoutStartSec\\b"), QStringLiteral("\\bDefaultTimeoutStopSec\\b"), QStringLiteral("\\bDefaultTimerAccuracySec\\b"),
        QStringLiteral("\\bDNS\\b"), QStringLiteral("\\bDNSOverTLS\\b"), QStringLiteral("\\bDNSSEC\\b"),
        QStringLiteral("\\bDNSStubListener\\b"), QStringLiteral("\\bDNSStubListenerExtra\\b"), QStringLiteral("\\bDomains\\b"),
        QStringLiteral("\\bDUIDRawData\\b"), QStringLiteral("\\bDUIDType\\b"), QStringLiteral("\\bDumpCore\\b"),
        QStringLiteral("\\bExternalSizeMax\\b"), QStringLiteral("\\bFallbackDNS\\b"), QStringLiteral("\\bFallbackNTP\\b"),
        QStringLiteral("\\bForwardToConsole\\b"), QStringLiteral("\\bForwardToKMsg\\b"), QStringLiteral("\\bForwardToSyslog\\b"),
        QStringLiteral("\\bForwardToWall\\b"), QStringLiteral("\\bHandleHibernateKey\\b"), QStringLiteral("\\bHandleHibernateKeyLongPress\\b"),
        QStringLiteral("\\bHandleLidSwitch\\b"), QStringLiteral("\\bHandleLidSwitchDocked\\b"), QStringLiteral("\\bHandleLidSwitchExternalPower\\b"),
        QStringLiteral("\\bHandlePowerKey\\b"), QStringLiteral("\\bHandlePowerKeyLongPress\\b"), QStringLiteral("\\bHandleRebootKey\\b"),
        QStringLiteral("\\bHandleRebootKeyLongPress\\b"), QStringLiteral("\\bHandleSuspendKey\\b"), QStringLiteral("\\bHandleSuspendKeyLongPress\\b"),
        QStringLiteral("\\bHibernateDelaySec\\b"), QStringLiteral("\\bHibernateKeyIgnoreInhibited\\b"), QStringLiteral("\\bHibernateMode\\b"),
        QStringLiteral("\\bHibernateState\\b"), QStringLiteral("\\bHoldoffTimeoutSec\\b"), QStringLiteral("\\bHybridSleepMode\\b"),
        QStringLiteral("\\bHybridSleepState\\b"), QStringLiteral("\\bIdleAction\\b"), QStringLiteral("\\bIdleActionSec\\b"),
        QStringLiteral("\\bInhibitDelayMaxSec\\b"), QStringLiteral("\\bInhibitorsMax\\b"), QStringLiteral("\\bIPv6PrivacyExtensions\\b"),
        QStringLiteral("\\bJournalSizeMax\\b"), QStringLiteral("\\bKeepFree\\b"), QStringLiteral("\\bKExecWatchdogSec\\b"),
        QStringLiteral("\\bKillExcludeUsers\\b"), QStringLiteral("\\bKillOnlyUsers\\b"), QStringLiteral("\\bKillUserProcesses\\b"),
        QStringLiteral("\\bLidSwitchIgnoreInhibited\\b"), QStringLiteral("\\bLineMax\\b"), QStringLiteral("\\bLLMNR\\b"),
        QStringLiteral("\\bLogColor\\b"), QStringLiteral("\\bLogLevel\\b"), QStringLiteral("\\bLogLocation\\b"),
        QStringLiteral("\\bLogTarget\\b"), QStringLiteral("\\bLogTime\\b"), QStringLiteral("\\bManageForeignRoutes\\b"),
        QStringLiteral("\\bManageForeignRoutingPolicyRules\\b"), QStringLiteral("\\bMaxFileSec\\b"), QStringLiteral("\\bMaxLevelConsole\\b"),
        QStringLiteral("\\bMaxLevelKMsg\\b"), QStringLiteral("\\bMaxLevelStore\\b"), QStringLiteral("\\bMaxLevelSyslog\\b"),
        QStringLiteral("\\bMaxLevelWall\\b"), QStringLiteral("\\bMaxRetentionSec\\b"), QStringLiteral("\\bMaxUse\\b"),
        QStringLiteral("\\bMulticastDNS\\b"), QStringLiteral("\\bNAutoVTs\\b"), QStringLiteral("\\bNTP\\b"),
        QStringLiteral("\\bNUMAMask\\b"), QStringLiteral("\\bNUMAPolicy\\b"), QStringLiteral("\\bPollIntervalMaxSec\\b"),
        QStringLiteral("\\bPollIntervalMinSec\\b"), QStringLiteral("\\bPowerKeyIgnoreInhibited\\b"), QStringLiteral("\\bProcessSizeMax\\b"),
        QStringLiteral("\\bRateLimitBurst\\b"), QStringLiteral("\\bRateLimitIntervalSec\\b"), QStringLiteral("\\bReadEtcHosts\\b"),
        QStringLiteral("\\bReadKMsg\\b"), QStringLiteral("\\bRebootKeyIgnoreInhibited\\b"), QStringLiteral("\\bRebootWatchdogSec\\b"),
        QStringLiteral("\\bReloadLimitIntervalSec\\b"), QStringLiteral("\\bReserveVT\\b"), QStringLiteral("\\bResolveUnicastSingleLabel\\b"),
        QStringLiteral("\\bRootDistanceMaxSec\\b"), QStringLiteral("\\bRouteTable\\b"), QStringLiteral("\\bRuntimeDirectoryInodesMax\\b"),
        QStringLiteral("\\bRuntimeDirectorySize\\b"), QStringLiteral("\\bRuntimeKeepFree\\b"), QStringLiteral("\\bRuntimeMaxFiles\\b"),
        QStringLiteral("\\bRuntimeMaxFileSize\\b"), QStringLiteral("\\bRuntimeMaxUse\\b"), QStringLiteral("\\bRuntimeWatchdogSec\\b"),
        QStringLiteral("\\bSaveIntervalSec\\b"), QStringLiteral("\\bSeal\\b"), QStringLiteral("\\bSessionsMax\\b"),
        QStringLiteral("\\bShowStatus\\b"), QStringLiteral("\\bSpeedMeter\\b"), QStringLiteral("\\bSpeedMeterIntervalSec\\b"),
        QStringLiteral("\\bSplitMode\\b"), QStringLiteral("\\bStaleRetentionSec\\b"), QStringLiteral("\\bStatusUnitFormat\\b"),
        QStringLiteral("\\bStopIdleSessionSec\\b"), QStringLiteral("\\bStorage\\b"), QStringLiteral("\\bSuspendEstimationSec\\b"),
        QStringLiteral("\\bSuspendKeyIgnoreInhibited\\b"), QStringLiteral("\\bSuspendMode\\b"), QStringLiteral("\\bSuspendState\\b"),
        QStringLiteral("\\bSwapUsedLimit\\b"), QStringLiteral("\\bSyncIntervalSec\\b"), QStringLiteral("\\bSystemKeepFree\\b"),
        QStringLiteral("\\bSystemMaxFiles\\b"), QStringLiteral("\\bSystemMaxFileSize\\b"), QStringLiteral("\\bSystemMaxUse\\b"),
        QStringLiteral("\\bTimerSlackNSec\\b"), QStringLiteral("\\bUnlink\\b"), QStringLiteral("\\bUserStopDelaySec\\b"),
        QStringLiteral("\\bWatchdogDevice\\b"),
    };
    for (const QString &pattern : systemdConfigKeywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = systemdConfigKeywordFormat;
        highlightingRules.append(rule);
    }

    //singleLineSystemdCommentFormat.setFontWeight(QFont::Bold);
    singleLineSystemdCommentFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = singleLineSystemdCommentFormat;
    highlightingRules.append(rule);

    //squareBracketFormat.setFontWeight(QFont::Bold);
    squareBracketFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\[.*\\]"));
    rule.format = squareBracketFormat;
    highlightingRules.append(rule);

    //classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    //commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentStartExpression = QRegularExpression(QStringLiteral("(^|\\s+)/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
